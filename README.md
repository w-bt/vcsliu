# vcsliu

![Image of AutoVCS](https://i.imgur.com/SKFuzyG.jpg)

A (2,2)-Visual Cryptography with Liu's method (single secret image) using C++ and openCV.

Pixel tolerance: 2 pixels

M0 = [[ 1, 1, 0, 0], [ 1, 1, 0, 0]]

M1 = [[ 1, 1, 0, 0], [ 0, 0, 1, 1]]


Pixel tolerance: 3 pixels

M0 = [[1, 1, 1, 0, 0, 0, 0, 0, 1], [1, 1, 1, 0, 0, 0, 0, 0, 1]]

M1 = [[1, 1, 1, 0, 0, 0, 0, 0, 1}, [0, 0, 0, 1, 1, 1, 0, 0, 1]]


Pixel tolerance: 8 pixels

M0 = [[ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0], [ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]]

M1 = [[ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0], [ 0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1]]

For bulk image please visit: [AutoVCS](https://github.com/w-bt/autovcsliu)
