# Databases

All files are gzipped

  - [x] [All 0-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmdozEnHBJjHRnHNhxNgJ4JM4VrVL3mfFVJNE6U4L9YVPm)
  - [x] [All 1-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmUq9NFE3oAPPXVE78Sy54dpBjiFQ2azpocXb9on9XA4oD)
  - [x] [All 2-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmWWKdPs4NpzgZjvFgSZtZb1sgPVmBXS3AErXsnucKwRuV)
  - [x] [All 3-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmNUUmRPTeRa63R7ceoQtisAjC54JaNjhk2PDXbDV1zZQq)
  - [x] [All 4-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmdgSWHK49c8zRYcbgrGU1ex3coQ4tHGeHmjypQdaeb6Go)
  - [x] [All 5-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmQgyDwDdv23n22B3PenK8sqSaAh4aK5M13LVLTPNJaeyC)
  - [x] [All 6-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmV7dHoT6L4PGN2uAGc7mN4bxvV5k8LTzkpNQRxbGCKitF)
  - [x] [All 7-dimensional 2-level polytopes](https://ipfs.io/ipfs/Qmc4pZZhEdHhbkdnToCBAJDf33Zk18nebSRpHfscejy74A)
  - [x] [All 8-dimensional 2-level polytopes](https://ipfs.io/ipfs/QmZj7JLMHF7Lr39mjQ2YfPrZwpqkipyWotGV8sBJZUPfxM)
  - [ ] All 9-dimensional two-level polytopes (pending ...)
  
For example
```
$ curl https://ipfs.io/ipfs/QmNUUmRPTeRa63R7ceoQtisAjC54JaNjhk2PDXbDV1zZQq | gunzip                                                 
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   103  100   103    0     0     20      0  0:00:05  0:00:05 --:--:--    24
3 4 4 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
3 8 6 1 0 0 0 1 1 0 1 0 0 1 1 1 0 1 0 1 0 1 0 0 1 0 1 0 1 1 0 1 0 0 1 0 1 0 1 1 0 1 1 0 0 0 1 1 1 0 0
3 5 5 1 0 0 0 0 0 1 0 0 1 0 0 1 0 1 0 1 0 1 0 0 0 1 1 0
3 5 6 1 0 0 0 0 1 0 1 0 0 1 1 0 0 1 0 1 0 0 1 0 1 0 0 1 0 1 1 0 0
3 6 8 1 0 0 0 0 1 1 1 0 1 0 0 1 0 1 1 0 0 1 0 1 1 0 1 1 1 0 1 0 0 1 0 0 1 1 1 1 0 0 0 1 0 1 1 0 1 0 0
```
yields the five 3-dimensional 2-level polytopes.

# New version

- to ATOMIZE the atoms

	python3 atomize.py {2,3,4,5,6}d.txt

- 6-dimensional popcorn machine!

	cat atoms/5/all | time ./run/inflate | ./run/dedup | ./run/sift atoms/5/all | wc -l


# Old version

- install nauty, available at http://pallini.di.uniroma1.it/#howtogetit 

- insert the absolute path to the folder /nauty26r7 into the NAUTYHOME variable in the makefile

- run make into the main directory of the code

- the executable takes in input from 1 to 4 arguments:
	
	./2level-enum D first_base last_base verbose_flag

	* the first argument is mandatory and is the dimension D you want to enumerate
	* the second and the third arguments (optional) first_base and last_base are the first and the last (D-1)-dimensional bases that you want to assign: in this way the program will enumerate all 2-level D-polytopes with bases betweet the first and the last base.
	By default, this parameters are set to include all (D-1)-dimensional 2-level polytopes. 
	* the fourth and last argument is optional and determines the verbose depth. It goes from 0 to 2 with 0 = minimal output, 2 = list of all closed sets and points in the embeddings of the ground set. By default verbose_flag = 0

	Examples of valid arguments:
	./2level-enum 3
	enumerates all 3-dimensional 2-level polytopes. It uses the default values for the remaining arguments.

	./2level-enum 3 - - 2
	enumerates all 3-dimensional 2-level polytopes. It uses the default values for first_base and last_base and provides maximal verbosity in the output.

	./2level-enum 3 0 0 2
	enumerates all 3-dimensional 2-level polytopes with the simplex as base (and with facets having less than or equal to 3 vertices)

The program outputs a list of 2-level D-polytopes in a txt file.

Notice that the code takes in input the list of (D-1)-polytopes in a txt file. The list for dimension 2 in provided in the repo.


