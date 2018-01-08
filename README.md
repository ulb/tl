# Databases

All files are gzipped

  | Dimension | Status  | Number  | File                                                                        |
  | --------- | ------- | ------- | --------------------------------------------------------------------------- |
  |         0 |    Done |       1 | [gzip](https://ipfs.io/ipfs/QmbTCfFWSZw6gGKYN6znFEnN3p9x8fW5SEk3pg3f3aedtf) |
  |         1 |    Done |       1 | [gzip](https://ipfs.io/ipfs/QmXttXoXYAq72HrThyy9DoFNAZMFpGiXBhTUjcHP62hkJD) |
  |         2 |    Done |       2 | [gzip](https://ipfs.io/ipfs/QmYQ1NBCRyrEYCjTEf7367ZEK6ScNn6qZAfnZM3ofoMXBB) |
  |         3 |    Done |       5 | [gzip](https://ipfs.io/ipfs/QmY8cnV5WuWDXw8dQWH85K4wVDGxENPij6yFhUB2CZwb7v) |
  |         4 |    Done |      19 | [gzip](https://ipfs.io/ipfs/QmeZZ9zcJnzwNjnxYL6rEvU3LBsPneuaSr4kYR9U2BQAo9) |
  |         5 |    Done |     106 | [gzip](https://ipfs.io/ipfs/QmP5Wmo6MTciWtvphifEDW5FGdwvTAaCJuAhHD8jjXTkiG) |
  |         6 |    Done |    1150 | [gzip](https://ipfs.io/ipfs/QmQekyWQajSMfEJQy8HsFL3FWxQ4n69gxJaXrWC3v8MPTF) |
  |         7 |    Done |   27292 | [gzip](https://ipfs.io/ipfs/QmVWnrQsH8sAWfN8WvVDJaffpVU7FRmfhGUFgh4MnhLRTh) |
  |         8 |    Done | 1378447 | [gzip](https://ipfs.io/ipfs/Qmawgxfpa9mjCk1RcfUm5SbckHvXv8XEqEZ78TuivEPuYt) |
  |         9 |       ? |       ? | ?                                                                           |

For example

    $ curl -Ss https://ipfs.io/ipfs/QmY8cnV5WuWDXw8dQWH85K4wVDGxENPij6yFhUB2CZwb7v | gunzip
    g 3 4 4 1000010000100001
    g 3 8 6 100011010011101010100101011010010101101100011100
    g 3 5 5 1000001001001010101000110
    g 3 5 6 100001010011001010010100101100
    g 3 6 8 100001110100101100101101110100100111100010110100

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

    $ echo 'g 0 1 1 1' > atoms/0/all

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
