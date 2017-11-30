# Databases

All files are gzipped

  | Dimension | Status  | Number  | File                                                                        |
  | --------- | ------- | ------- | --------------------------------------------------------------------------- |
  |         0 |    Done |       1 | [gzip](https://ipfs.io/ipfs/QmdozEnHBJjHRnHNhxNgJ4JM4VrVL3mfFVJNE6U4L9YVPm) |
  |         1 |    Done |       1 | [gzip](https://ipfs.io/ipfs/QmUq9NFE3oAPPXVE78Sy54dpBjiFQ2azpocXb9on9XA4oD) |
  |         2 |    Done |       2 | [gzip](https://ipfs.io/ipfs/QmWWKdPs4NpzgZjvFgSZtZb1sgPVmBXS3AErXsnucKwRuV) |
  |         3 |    Done |       5 | [gzip](https://ipfs.io/ipfs/QmNUUmRPTeRa63R7ceoQtisAjC54JaNjhk2PDXbDV1zZQq) |
  |         4 |    Done |      19 | [gzip](https://ipfs.io/ipfs/QmdgSWHK49c8zRYcbgrGU1ex3coQ4tHGeHmjypQdaeb6Go) |
  |         5 |    Done |     106 | [gzip](https://ipfs.io/ipfs/QmQgyDwDdv23n22B3PenK8sqSaAh4aK5M13LVLTPNJaeyC) |
  |         6 |    Done |    1150 | [gzip](https://ipfs.io/ipfs/QmV7dHoT6L4PGN2uAGc7mN4bxvV5k8LTzkpNQRxbGCKitF) |
  |         7 |    Done |   27292 | [gzip](https://ipfs.io/ipfs/Qmc4pZZhEdHhbkdnToCBAJDf33Zk18nebSRpHfscejy74A) |
  |         8 |    Done | 1378447 | [gzip](https://ipfs.io/ipfs/QmZj7JLMHF7Lr39mjQ2YfPrZwpqkipyWotGV8sBJZUPfxM) |
  |         9 |       ? |       ? | ?                                                                           |

For example

    $ curl -Ss https://ipfs.io/ipfs/QmNUUmRPTeRa63R7ceoQtisAjC54JaNjhk2PDXbDV1zZQq | gunzip
    3 4 4 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
    3 8 6 1 0 0 0 1 1 0 1 0 0 1 1 1 0 1 0 1 0 1 0 0 1 0 1 0 1 1 0 1 0 0 1 0 1 0 1 1 0 1 1 0 0 0 1 1 1 0 0
    3 5 5 1 0 0 0 0 0 1 0 0 1 0 0 1 0 1 0 1 0 1 0 0 0 1 1 0
    3 5 6 1 0 0 0 0 1 0 1 0 0 1 1 0 0 1 0 1 0 0 1 0 1 0 0 1 0 1 1 0 0
    3 6 8 1 0 0 0 0 1 1 1 0 1 0 0 1 0 1 1 0 0 1 0 1 1 0 1 1 1 0 1 0 0 1 0 0 1 1 1 1 0 0 0 1 0 1 1 0 1 0 0

yields the five 3-dimensional 2-level polytopes.

# Running the program

## Setup

### Install gcc or clang

For compiling.

### Installing nauty

For characteristic vector computation. Nauty is available
[here](http://pallini.di.uniroma1.it/#howtogetit). Download the latest version,
compile it and create a symlink from `~/.opt/nauty`.

### Clone the repository

    $ git clone git@github.com:ulb/tl

## Compiling

Jump inside the tl directory

    $ cd tl

For clang

    $ make

For gcc

    $ make CXX=g++

## Running

Create directories to store all generated polytopes

    $ mkdir -p atoms/{$(seq 0 9)}

Generate all 0-dimensional 2-level polytopes (a single vertex)

    $ echo '0 1 1 1' > atoms/0/all

Generate all 1-dimensional 2-level polytopes

    $ cat atoms/0/all | ./run/inflate | ./run/dedup | ./run/sift atoms/0/all > atoms/1/all

Generate all 2-dimensional 2-level polytopes

    $ cat atoms/1/all | ./run/inflate | ./run/dedup | ./run/sift atoms/1/all > atoms/2/all

Et cetera. A script does all that for you:

    $ sh enum.sh 6
    1
    2
    5
    19
    106
    1150

# Tools for transpiling from the old format to the new one

    python3 atomize.py {2,3,4,5,6}d.txt
