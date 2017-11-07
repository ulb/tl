#!/usr/bin/env python3

import os
import sys

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

        i = 0

        outdir = 'atoms/{}'.format(dimension)

        os.makedirs(outdir, exist_ok=True)

        out = outdir + '/all'

        with open(filename) as fd, open(out, 'w') as _all:

            # matrix = ()

            matrix = ''
            columns = 0
            rows = 0

            for line in fd:

                line = line.rstrip('\n')

                if line == '-' :

                    data = '{d} {r} {c} {m}\n'.format(
                        d=dimension,
                        r=rows,
                        c=columns,
                        m=' '.join(matrix),
                    )

                    _all.write(data)

                    matrix = ''
                    rows = 0
                    columns = 0

                    i+=1
                    if i % 1000 == 0 : print(i)

                    # atom = Atom(dimension, matrix)
                    # atom.dump(_all)
                    # matrix = ()

                else :

                    columns = len(line)
                    rows += 1
                    matrix += line
                    # matrix += ( tuple(map(int, line)) , )

        print(i)

        # for i, atom in enumerate(atoms):
            # with open('atoms/{}/{}'.format(atom.dimension,i), 'w') as fd:
                # atom.dump(fd, linesep = '\n', itemsep = '\n' )

