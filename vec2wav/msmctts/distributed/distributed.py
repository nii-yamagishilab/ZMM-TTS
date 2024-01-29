from torch.autograd import Variable
from torch.nn.modules import Module

import os
import sys
import time
import subprocess
import argparse

import torch
import torch.distributed as dist


def reduce_tensor(tensor, num_gpus):
    rt = tensor.clone()
    dist.all_reduce(rt, op=dist.reduce_op.SUM)
    rt /= num_gpus
    return rt


def init_distributed(rank, num_gpus, group_name, dist_backend, dist_url):
    assert torch.cuda.is_available(), "Distributed mode requires CUDA."
    print("Initializing Distributed")

    # Set cuda device so everything is done on the right GPU.
    torch.cuda.set_device(rank % num_gpus)

    # Initialize distributed communication
    dist.init_process_group(dist_backend, init_method=dist_url,
                            world_size=num_gpus, rank=rank,
                            group_name=group_name)


def _flatten_dense_tensors(tensors):
    """Flatten dense tensors into a contiguous 1D buffer. Assume tensors are of
    same dense type.
    Since inputs are dense, the resulting tensor will be a concatenated 1D
    buffer. Element-wise operation on this buffer will be equivalent to
    operating individually.
    Arguments:
        tensors (Iterable[Tensor]): dense tensors to flatten.
    Returns:
        A contiguous 1D buffer containing input tensors.
    """
    if len(tensors) == 1:
        return tensors[0].contiguous().view(-1)
    flat = torch.cat([t.contiguous().view(-1) for t in tensors], dim=0)
    return flat


def _unflatten_dense_tensors(flat, tensors):
    """View a flat buffer using the sizes of tensors. Assume that tensors are of
    same dense type, and that flat is given by _flatten_dense_tensors.
    Arguments:
        flat (Tensor): flattened dense tensors to unflatten.
        tensors (Iterable[Tensor]): dense tensors whose sizes will be used to
          unflatten flat.
    Returns:
        Unflattened dense tensors with sizes same as tensors and values from
        flat.
    """
    outputs = []
    offset = 0
    for tensor in tensors:
        numel = tensor.numel()
        outputs.append(flat.narrow(0, offset, numel).view_as(tensor))
        offset += numel
    return tuple(outputs)


'''
This version of DistributedDataParallel is designed to be used in conjunction with the multiproc.py
launcher included with this example. It assumes that your run is using multiprocess with 1
GPU/process, that the model is on the correct device, and that torch.set_device has been
used to set the device.
Parameters are broadcasted to the other processes on initialization of DistributedDataParallel,
and will be allreduced at the finish of the backward pass.
'''
class DistributedDataParallel(Module):

    def __init__(self, module):
        super(DistributedDataParallel, self).__init__()
        #fallback for PyTorch 0.3
        if not hasattr(dist, '_backend'):
            self.warn_on_half = True
        else:
            self.warn_on_half = True if dist._backend == dist.dist_backend.GLOO else False

        self.module = module

        for p in self.module.state_dict().values():
            if not torch.is_tensor(p):
                continue
            dist.broadcast(p, 0)

        def allreduce_params():
            if(self.needs_reduction):
                self.needs_reduction = False
                buckets = {}
                for param in self.module.parameters():
                    if param.requires_grad and param.grad is not None:
                        tp = type(param.data)
                        if tp not in buckets:
                            buckets[tp] = []
                        buckets[tp].append(param)
                if self.warn_on_half:
                    if torch.cuda.HalfTensor in buckets:
                        print("WARNING: gloo dist backend for half parameters may be extremely slow." +
                              " It is recommended to use the NCCL backend in this case. This currently requires" +
                              "PyTorch built from top of tree master.")
                        self.warn_on_half = False

                for tp in buckets:
                    bucket = buckets[tp]
                    grads = [param.grad.data for param in bucket]
                    coalesced = _flatten_dense_tensors(grads)
                    dist.all_reduce(coalesced)
                    coalesced /= dist.get_world_size()
                    for buf, synced in zip(grads, _unflatten_dense_tensors(coalesced, grads)):
                        buf.copy_(synced)

        for param in list(self.module.parameters()):
            def allreduce_hook(*unused):
                param._execution_engine.queue_callback(allreduce_params)
            if param.requires_grad:
                param.register_hook(allreduce_hook)

    def forward(self, *inputs, **kwargs):
        self.needs_reduction = True
        return self.module(*inputs, **kwargs)

    '''
    def _sync_buffers(self):
        buffers = list(self.module._all_buffers())
        if len(buffers) > 0:
            # cross-node buffer sync
            flat_buffers = _flatten_dense_tensors(buffers)
            dist.broadcast(flat_buffers, 0)
            for buf, synced in zip(buffers, _unflatten_dense_tensors(flat_buffers, buffers)):
                buf.copy_(synced)
     def train(self, mode=True):
        # Clear NCCL communicator and CUDA event cache of the default group ID,
        # These cache will be recreated at the later call. This is currently a
        # work-around for a potential NCCL deadlock.
        if dist._backend == dist.dist_backend.NCCL:
            dist._clear_group_cache()
        super(DistributedDataParallel, self).train(mode)
        self.module.train(mode)
    '''
'''
Modifies existing model to do gradient allreduce, but doesn't change class
so you don't need "module"
'''
def apply_gradient_allreduce(module):
    if not hasattr(dist, '_backend'):
        module.warn_on_half = True
    else:
        module.warn_on_half = True if dist._backend == dist.dist_backend.GLOO else False

    for p in module.state_dict().values():
        if not torch.is_tensor(p):
            continue
        dist.broadcast(p, 0)

    def allreduce_params():
        if module.needs_reduction:
            module.needs_reduction = False
            buckets = {}
            for param in module.parameters():
                if param.requires_grad and param.grad is not None:
                    tp = param.data.dtype
                    if tp not in buckets:
                        buckets[tp] = []
                    buckets[tp].append(param)
            if module.warn_on_half:
                if torch.cuda.HalfTensor in buckets:
                    print("WARNING: gloo dist backend for half parameters may be extremely slow." +
                            " It is recommended to use the NCCL backend in this case. This currently requires" +
                            "PyTorch built from top of tree master.")
                    module.warn_on_half = False

            for tp in buckets:
                bucket = buckets[tp]
                grads = [param.grad.data for param in bucket]
                coalesced = _flatten_dense_tensors(grads)
                dist.all_reduce(coalesced)
                coalesced /= dist.get_world_size()
                for buf, synced in zip(grads, _unflatten_dense_tensors(coalesced, grads)):
                    buf.copy_(synced)

    for param in list(module.parameters()):
        def allreduce_hook(*unused):
            Variable._execution_engine.queue_callback(allreduce_params)
        if param.requires_grad:
            param.register_hook(allreduce_hook)

    def set_needs_reduction(self, input, output):
        self.needs_reduction = True
        module.needs_reduction = True

    module.register_forward_hook(set_needs_reduction)
    for sub_module_name in module._modules.keys():
        module._modules[sub_module_name].register_forward_hook(set_needs_reduction)
    return module


def apply_gradient_allreduce_v0(module):
    module.call_times = 0
    module.needs_reduction = True
    if not hasattr(dist, '_backend'):
        module.warn_on_half = True
    else:
        module.warn_on_half = True if dist._backend == dist.dist_backend.GLOO else False

    for p in module.state_dict().values():
        if not torch.is_tensor(p):
            continue
        dist.broadcast(p, 0)

    def allreduce_params():
        if module.call_times % len(module.parameters()) == 0:
            module.call_times = 0
            module.needs_reduction = True
        
        if not module.needs_reduction:
            return
        
        module.call_times += 1
        module.needs_reduction = False

        buckets = {}
        for param in module.parameters():
            if param.requires_grad and param.grad is not None:
                tp = param.data.dtype
                if tp not in buckets:
                    buckets[tp] = []
                buckets[tp].append(param)
        if module.warn_on_half:
            if torch.cuda.HalfTensor in buckets:
                print("WARNING: gloo dist backend for half parameters may be extremely slow." +
                        " It is recommended to use the NCCL backend in this case. This currently requires" +
                        "PyTorch built from top of tree master.")
                module.warn_on_half = False

        for tp in buckets:
            bucket = buckets[tp]
            grads = [param.grad.data for param in bucket]
            coalesced = _flatten_dense_tensors(grads)
            dist.all_reduce(coalesced)
            coalesced /= dist.get_world_size()
            for buf, synced in zip(grads, _unflatten_dense_tensors(coalesced, grads)):
                buf.copy_(synced)

    for param in list(module.parameters()):
        def allreduce_hook(*unused):
            Variable._execution_engine.queue_callback(allreduce_params)
        if param.requires_grad:
            param.register_hook(allreduce_hook)

    return module