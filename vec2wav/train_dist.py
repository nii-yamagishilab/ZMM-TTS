from argparse import ArgumentParser, REMAINDER

import argparse
import os
import sys
import subprocess
import time
import torch


training_script = os.path.join(os.path.dirname(__file__), 'train.py')


def multi_gpu(stdout_dir, training_script_args):
    args_list = [training_script, '-c', training_script_args]
    
    num_gpus = torch.cuda.device_count()
    args_list.append('--num_gpus={}'.format(num_gpus))
    args_list.append("--group_name=group_{}".format(time.strftime("%Y_%m_%d-%H%M%S")))

    if not os.path.isdir(stdout_dir):
        os.makedirs(stdout_dir)
        os.chmod(stdout_dir, 0o775)

    workers = []

    for i in range(num_gpus):
        args_list[-2] = '--rank={}'.format(i)
        stdout = None if i == 0 else open(
            os.path.join(stdout_dir, "GPU_{}.log".format(i)), "w")
        print(args_list)
        p = subprocess.Popen([str(sys.executable)]+args_list, stdout=stdout)
        workers.append(p)

    for p in workers:
        p.wait()


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-s', '--stdout_dir', type=str,
                        default="/tmp/msmc-tts/logs",
                        help='directory to save stoud logs')
    parser.add_argument('-c', '--config_file', type=str,
                        help='File path to load model config')

    args = parser.parse_args()
    multi_gpu(args.stdout_dir, args.config_file)


if __name__ == '__main__':
    main()