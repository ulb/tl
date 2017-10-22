#!/usr/bin/env python3

import os
import sys
from hashlib import sha256
from base64 import urlsafe_b64encode

class Atom (object):

    def __init__ ( self , dimension , matrix ) :

        assert(isinstance(matrix, tuple))

        rows = len(matrix)

        assert(rows > 0)
        assert(isinstance(matrix[0], tuple))

        columns = len(matrix[0])

        assert(all(len(row) == columns for row in matrix))

        self.dimension = dimension
        self.rows = rows
        self.columns = columns
        self.matrix = matrix
        self.hash = urlsafe_b64encode(sha256(self.data().encode('utf-8')).digest()).decode('utf-8')

    def data ( self ) :

        return '{}-{}x{}-{}'.format(self.dimension, self.rows, self.columns,
                ''.join(map(lambda row : ''.join(map(str,row)), self.matrix)))

    def dump ( self , fd ) :

        fd.write('{}\n'.format(self.dimension))
        fd.write('{}\n'.format(self.rows))
        fd.write('{}\n'.format(self.columns))
        for row in self.matrix:
            fd.write('{}\n'.format(' '.join(map(str,row))))

    def __repr__ ( self ) :

        return '{}-{}x{}-{}'.format(self.dimension, self.rows, self.columns, self.hash)


if __name__ == '__main__':

    for filename in sys.argv[1:]:

        dimension = int(filename[0]) # parse dimension

        atoms = []

        with open(filename) as fd:

            matrix = ()

            for line in fd.read().splitlines():

                if line == '-' :
                    atoms.append( Atom(dimension, matrix) )
                    matrix = ()
                else :
                    matrix += ( tuple(map(int, line)) , )

        os.makedirs('atoms/{}'.format(dimension), exist_ok=True)

        for i, atom in enumerate(atoms):
            with open('atoms/{}/{}'.format(atom.dimension,i), 'w') as fd:
                atom.dump(fd)
