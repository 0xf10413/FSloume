#!/usr/bin/env python3
"""
Transforme une ressource en .c.
Représente les octets dans une string, avec une taille max
d'octets par tableau.
Regroupe le tout dans un tableau de ces tableaux.
TODO:
    création d'un .h qui liste toutes les données
"""
from sys import argv, exit

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


def usage():
    print(argv[0] + " : transforms files into .c")
    print("Usage : {} input [output]".format(argv[0]))

if len(argv) < 2:
    usage()
    exit(1)


rc_filename = argv[1]
if len(argv) > 2:
    c_filename = argv[2]
else:
    c_filename = argv[1] + ".c"

var_name = argv[1].replace(".", "_").replace('/', '_')

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
        self.master.file.write('char {}_{} [] =\n'. format(
            self.master.rc_var_name,
            self.master.index_of_cur_array,
            ))
        self.opened = True

    def end(self):
        if not self.opened:
            return
        self.master.file.write(';\n')
        self.opened = False

class CResource(object):
    """
    Une ressource C-isée
    """

    def __init__(self, c_filename, rc_filename):
        self.file = open(c_filename, 'w')
        self.rc_var_name = rc_filename.replace(".", "_").replace('/', '_')
        self.bytes_in_cur_line = 0
        self.bytes_in_cur_array = 0
        self.index_of_cur_array = 0
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
        self.file.write("{}".format(hex(byte).replace('0', '\\',1)))
        self.bytes_in_cur_line += 1
        self.bytes_in_cur_array += 1

    def _conclude(self):
        """
        Achève le fichier en insérant un tableau des tableaux
        """
        self.cstr.end()
        self.carray.end()
        self.file.write('char *{} [{}] = {}\n'.
                format(self.rc_var_name, self.index_of_cur_array+1, '{'))
        for i in range(self.index_of_cur_array+1):
            self.file.write("{}{}_{},\n".format(
                INDENT, self.rc_var_name, i,
                ))
        self.file.write('};\n')

    def get_declaration(self):
        """
        Renvoie la déclaration à mettre dans le .h
        """
        return "char *{} [{}];".format(
                self.rc_var_name,
                self.index_of_cur_array+1,
                )

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self._conclude()
        self.file.close()

with open(rc_filename, 'rb') as r, CResource(c_filename, rc_filename) as w:
    for byteline in r:
        for byte in byteline:
            w.add(byte)
