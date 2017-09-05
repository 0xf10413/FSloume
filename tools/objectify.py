#!/usr/bin/env python3
"""
Transforme une ressource en .c
TODO:
    split des strings si le fichier est trop gros
    création d'un .h qui liste toutes les données
    ajouter la taille et un octet nul de fin de string
"""

from sys import argv, exit

def usage():
    print(argv[0] + " : transforms files into .c")
    print("Usage : {} input [output]".format(argv[0]))

if len(argv) < 2:
    usage()
    exit(1)

MAX_BYTES_PER_LINE = 5

rc_filename = argv[1]
if len(argv) > 2:
    c_filename = argv[2]
else:
    c_filename = argv[1] + ".c"

var_name = argv[1].replace(".", "_").replace('/', '_')
indent = ' '*3

with open(rc_filename, 'rb') as r, \
     open(c_filename, 'w') as w:
        bytes_in_line = 0
        w.write('char {} [] =\n{}"'.format(var_name, indent))
        open_text = True
        for byteline in r:
            for byte in byteline:
                w.write("{}".format(hex(byte).replace('0', '\\',1)))
                bytes_in_line += 1
                if bytes_in_line > MAX_BYTES_PER_LINE:
                    w.write('"\n{}"'.format(indent))
                    bytes_in_line = 0
        w.write('"\n;\n')
