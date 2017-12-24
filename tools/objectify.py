#!/usr/bin/env python3
"""
En mode rc2c :
    Transforme une ressource en .c.
    Représente les octets dans une string, avec une taille max
    d'octets par tableau.
    Regroupe le tout dans un tableau de ces tableaux.
En mode rc2h :
    Construit un .h avec les déclarations nécessaires pour réutiliser
    les .c.
"""
from sys import exit, stdout
from glob import iglob
from os.path import getsize

import argparse

### Paramètres ###

# Esthétique : nombre max de bytes dans une ligne
MAX_BYTES_PER_LINE = 5
# Esthétique : degré d'indentation
INDENT = ' '*3
# Pratique : nombre max de bytes à mettre dans un array
# avant de passer au suivant
# Attention, cette taille est limitée par le compilateur
MAX_BYTES_PER_ARRAY = 4000

### Fin des paramètres ###


parser = argparse.ArgumentParser(
        description="Changes ressource files into C files")
parser.add_argument('mode', choices=['rc2c','rc2h','rc2cpp'],
        help="rc2c : resource to C file, "
        " rc2h : resource folder to C header"
        " rc2cpp : ressource folder to C++ ressource manager source")
parser.add_argument('input')
parser.add_argument('--output', help="Output file, default is stdout")

args = parser.parse_args()

rc_filename = args.input
if args.output is None:
    c_filename = '-'
else:
    c_filename = args.output

var_name = rc_filename.replace(".", "_").replace('/', '_')

class CString(object):
    """
    Une lignes de bytes C-isée
    """
    def __init__(self, master):
        self.master = master
        self.opened = False

    def begin(self):
        if self.opened:
            return
        self.master.file.write(INDENT + '"')
        self.opened = True

    def end(self):
        if not self.opened:
            return
        self.master.file.write('"\n')
        self.opened = False

class CArray(object):
    """
    Une variable C, aggrégat de CLinestring
    """
    def __init__(self, master):
        self.master = master
        self.opened = False

    def begin(self):
        if self.opened:
            return
        self.master.file.write('const char {}_{} [] =\n'. format(
            self.master.rc_var_name,
            self.master.index_of_cur_array,
            ))
        self.opened = True

    def end(self):
        if not self.opened:
            return
        self.master.file.write(';\n\n')
        self.opened = False

class CResource(object):
    """
    Une ressource C-isée
    """
    def __init__(self, c_filename, rc_filename):
        self.file = stdout if c_filename == '-' else open(c_filename, 'w')
        self.total_size = getsize(rc_filename)
        self.rc_var_name = rc_filename.replace(".", "_").replace('/', '_')
        self.bytes_in_cur_line = 0
        self.bytes_in_cur_array = 0
        self.index_of_cur_array = 0
        self.file.write("#include <stddef.h> // For size_t\n\n")
        self.cstr = CString(self)
        self.carray = CArray(self)
        self.carray.begin()
        self.cstr.begin()

    def add(self, byte):
        # Premier test : si le tableau est plein, on passe au suivant
        if self.bytes_in_cur_array == MAX_BYTES_PER_ARRAY:
            self.cstr.end()
            self.carray.end()

            self.bytes_in_cur_line = 0
            self.bytes_in_cur_array = 0
            self.index_of_cur_array += 1

            self.carray.begin()
            self.cstr.begin()
        # Deuxième test : si la ligne est pleine, on passe à la suivante
        elif self.bytes_in_cur_line == MAX_BYTES_PER_LINE:
            self.cstr.end()
            self.bytes_in_cur_line = 0
            self.cstr.begin()
        # On peut insérer en toute sérénité
        self.file.write("{0:#04x}".format(byte).replace('0','\\', 1))
        self.bytes_in_cur_line += 1
        self.bytes_in_cur_array += 1

    def _conclude(self):
        """
        Achève le fichier en insérant un tableau des tableaux
        puis en notant la taille totale
        """
        self.cstr.end()
        self.carray.end()
        self.file.write('const char *{} [{}] = {}\n'.
                format(self.rc_var_name, self.index_of_cur_array+2, '{'))
        for i in range(self.index_of_cur_array+1):
            self.file.write("{}{}_{},\n".format(
                INDENT, self.rc_var_name, i,
                ))
        self.file.write("{}0,\n".format(
            INDENT,
            ))
        self.file.write('};\n\n')
        self.file.write('const size_t {} = {};\n'.
                format(self.rc_var_name + '_size', self.total_size)
                )


    def __enter__(self):
        """
        Appelée à l'entrée d'un contexte
        Aucun traitement spécifique à effectuer
        """
        return self

    def __exit__(self, type, value, traceback):
        """
        Appelée à la sortie d'un contexte
        S'assure que le fichier est correctement conclu et fermé
        """
        self._conclude()
        self.file.close()

def objectify():
    with open(rc_filename, 'rb') as r, CResource(c_filename, rc_filename) as w:
        for byteline in r:
            for byte in byteline:
                w.add(byte)

def headerify():
    file = stdout if c_filename == "-" else open(c_filename, 'w')
    with file as w:
        w.write("#ifndef F_RC_H\n")
        w.write("#define F_RC_H\n\n")
        w.write("#include <stddef.h> // For size_t\n\n")
        w.write("#define RC_MAX_BYTES_PER_ARRAY {}\n\n".format(MAX_BYTES_PER_ARRAY))
        for i in iglob(rc_filename + "/*"):
            size = getsize(i)
            pieces = size // MAX_BYTES_PER_ARRAY + 2
            i = i.replace(".", "_").replace('/', '_')
            w.write("extern const char *{} [{}];\n".format(i, pieces))
            w.write("extern const size_t {};\n\n".format(i + '_size', size))
        w.write("#endif // defined F_RC_H\n")

def cppify():
    file = stdout if c_filename == "-" else open(c_filename, 'w')
    with file as w:
        first_iteration = True
        for i in iglob(rc_filename + "/*"):
            i = i.replace(".", "_").replace('/', '_')
            if first_iteration:
                w.write("if ")
                first_iteration = False
            else:
                w.write("else if ")
            w.write('(name == "{}")\n'.format(i))
            w.write('   return ResourceStream({}, {}_size);\n'.format(i,i))


if __name__ == "__main__":
    if args.mode == 'rc2c':
        objectify()
    elif args.mode == 'rc2h':
        headerify()
    elif args.mode == 'rc2cpp':
        cppify()
    else:
        raise NotImplementedError("Unimplemented mode " + mode)

