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

    def data ( self , linesep = '' , colsep = '' , itemsep = ' ' ) :

        return '{d}{s}{r}{s}{c}{s}{m}'.format(
            s=itemsep,
            d=self.dimension,
            r=self.rows,
            c=self.columns,
            m=linesep.join(map(lambda row : colsep.join(map(str,row)), self.matrix)),
        )

    def dump ( self , fd  , linesep = ' ' , colsep = ' ' , itemsep = ' ' ) :

        fd.write(self.data(linesep=linesep, colsep=colsep, itemsep=itemsep))
        fd.write('\n')

    def __repr__ ( self ) :

        return self.data()


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

        with open('atoms/{}/all'.format(dimension), 'w') as _all:
            for atom in atoms:
                atom.dump(_all)

        for i, atom in enumerate(atoms):
            with open('atoms/{}/{}'.format(atom.dimension,i), 'w') as fd:
                atom.dump(fd, linesep = '\n', itemsep = '\n' )

