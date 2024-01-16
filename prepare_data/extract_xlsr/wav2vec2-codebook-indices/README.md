# wav2vec2-codebook-indices

Just a playground repo to experiment extracting information about the wav2vec 2.0 quantization process. Run `python scripts/get_codebook_indices.py` to get a (N, 2) Numpy matrix (where N = number of time frames) of the form:

```
[[ 54 269]
 [146 284]
 [ 28  18]
  ... ...
 [118 111]
 [146 252]]
```

Where the two numbers represent the index in a codebook of size 320 (see the 'Quantization module' section of the [Illustrated Transformer](https://jonathanbgn.com/2021/09/30/illustrated-wav2vec-2.html#quantization-module)).

![wav2vec2_quantization_module](https://user-images.githubusercontent.com/9938298/171501185-cb9bcfaf-a5d5-4e7a-956c-21284385d3bc.png)
