# Database

All files are compressed

  | Dimension | Status | Number | # simplicial | # polar | # stab | # cs | Size | Download |
  | --------- | ------ | ------ | ------------ | ------- | ------ | ---- | ---- | -------- |
  | 0 | Done | 1 | 1 | 1 | 1 | 0 | 10B | [gzip (27B)](https://ipfs.io/ipfs/Qmb4HZF5JPQx4LD8PkhdJBku3YFYbz8bvvaATgtrorAHYi), [xz (68B)](https://ipfs.io/ipfs/QmbyrwKy4iGEnvdhNo3SQKwNCYYNYXeDJmnpCXZgMpq52S) |
  | 1 | Done | 1 | 1 | 1 | 1 | 1 | 13B | [gzip (31B)](https://ipfs.io/ipfs/QmWUmuoNREeaCYZ49DvxM2wg9VgRGpEDQyqAJHii9v9fhV), [xz (72B)](https://ipfs.io/ipfs/QmZ4qRDzmhFUMxuFC4raL259BbhqbMAJ6kkwqU4Zrer6hv) |
  | 2 | Done | 2 | 2 | 2 | 2 | 2 | 43B | [gzip (47B)](https://ipfs.io/ipfs/QmTnoXDxZU7oss8EYNiJ3sBgPn1H8dWE8hMZzR9M7iEGpc), [xz (88B)](https://ipfs.io/ipfs/QmQorFHAV6ho525uQDRivUJqXNKTMEcNA2iqGZut5TLDP8) |
  | 3 | Done | 5 | 4 | 4 | 4 | 2 | 212B | [gzip (100B)](https://ipfs.io/ipfs/QmQTT1VpGQLr5va86uy5xVCzPvFRuXDvH7taHfqfpQe6f6), [xz (140B)](https://ipfs.io/ipfs/QmPPK5rd5XcPQuMsEEvTdpb6DQcVg9GiquWCwTC4ryPeaj) |
  | 4 | Done | 19 | 12 | 12 | 11 | 4 | 2K | [gzip (351B)](https://ipfs.io/ipfs/QmTL6v8gyMVU5ivLx5VjxAPz7VMrsiLzFuxZdz6BqiL1PP), [xz (396B)](https://ipfs.io/ipfs/QmXTTSn1FewBtfXTboJeeh24Au7TsuF47e7V4fBQ84CKf6) |
  | 5 | Done | 106 | 41 | 42 | 33 | 13 | 16K | [gzip (2K)](https://ipfs.io/ipfs/QmQ38UwegXkMfXqXa9ymoUgtzxkDyRwiXNAPyZcQEMDZrJ), [xz (2K)](https://ipfs.io/ipfs/Qmdwa4MVoL1ZMUF7aZj4C2dSw6xNmEyWXV1gkbCk2KhhiH) |
  | 6 | Done | 1150 | 248 | 276 | 148 | 45 | 297K | [gzip (33K)](https://ipfs.io/ipfs/QmckTB7Atb75e3gFZh1ve4pi7QZfRAMc6KwG3Hb69d8h2Q), [xz (25K)](https://ipfs.io/ipfs/QmXeDfhTkz4tukPZDi9tQ7ZP9m7pYotSV8WtXmhurC4XWn) |
  | 7 | Done | 27292 | 2687 | 3526 | 906 | 238 | 11M | [gzip (899K)](https://ipfs.io/ipfs/QmfNJhmLHgjJY6KTjagfjJGdSE7z2YtThFd3sYzR684ooQ), [xz (651K)](https://ipfs.io/ipfs/Qmf6cEa2Gr2zVcKUeVJTzjchmVseciVxKvRgFKkRNmrjEf) |
  | 8 | Done | 1378453 | 55506 | 102868 | 8887 | 1790 | 868M | [gzip (52M)](https://ipfs.io/ipfs/QmVwTwaGAQkWMezXdSLjDCUkSXdo6qinbWjxk1ByWpfihH), [xz (38M)](https://ipfs.io/ipfs/QmUBFVgmdLE2dLdkTK4pR6pyJErx6FMtf8Aw3h1Bi9Svbu) |
  | 9 | ? | ? | ? | ? | ? | ? | ? | ? |


For example

    $ curl -Ss https://ipfs.io/ipfs/QmQTT1VpGQLr5va86uy5xVCzPvFRuXDvH7taHfqfpQe6f6 | gunzip
    g 3 4 4 1000010000100001
    g 3 8 6 100011010011101010100101011010010101101100011100
    g 3 5 5 1000001001001010101000110
    g 3 5 6 100001010011001010010100101100
    g 3 6 8 100001110100101100101101110100100111100010110100

and

    $ curl -Ss https://ipfs.io/ipfs/QmPPK5rd5XcPQuMsEEvTdpb6DQcVg9GiquWCwTC4ryPeaj | unxz
    g 3 4 4 1000010000100001
    g 3 8 6 100011010011101010100101011010010101101100011100
    g 3 5 5 1000001001001010101000110
    g 3 5 6 100001010011001010010100101100
    g 3 6 8 100001110100101100101101110100100111100010110100

each yield the five 3-dimensional 2-level polytopes.

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

    $ mkdir -p db/src

Generate all 0-dimensional 2-level polytopes (a single vertex)

    $ echo 'g 0 1 1 1' > db/src/0

Generate all 1-dimensional 2-level polytopes

    $ cat db/src/0 | ./run/inflate | ./run/dedup | ./run/sift db/src/0 > db/src/1

Generate all 2-dimensional 2-level polytopes

    $ cat db/src/1 | ./run/inflate | ./run/dedup | ./run/sift db/src/1 > db/src/2

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
