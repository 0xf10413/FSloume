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
from os.path import getsize

import argparse
import configparser
import re

### Paramètres ###

### Fin des paramètres ###


parser = argparse.ArgumentParser(
        description="Write some constants that can be initialized")
parser.add_argument('input_ini',
        help="INI file with all constants")
parser.add_argument('output_h',
        help="Header output")
parser.add_argument('output_cpp',
        help="Source output")

args = parser.parse_args()

ini_filename = args.input_ini
header_filename = args.output_h
source_filename = args.output_cpp


class ConfigHeader(object):
    def __init__(self, header, cg):
        self.cg = cg
        self.file = stdout if header == "-" else open(header, 'w')
        self.file.write("#ifndef F_CONFIG_H\n")
        self.file.write("#define F_CONFIG_H\n\n")

        self.write_headers()

        self.file.write("class Config\n{\n")
        self.file.write("private:\n")
        self.file.write("   void updateParams();\n")
        self.file.write("public:\n")
        self.file.write("   Config();\n\n")

    def parse(self):
        for sec in ["Master", "Init", "Depends"]:
            cur_sec = cg[sec]
            for var in cur_sec:
                type, expr = explode_expr(cur_sec[var])
                self.add_variable(sec, var.upper(), type, expr)
        self.conclude()


    def add_variable(self, kind, name, type, expr):
        """
        - kind : master, init, depends
        - name : nom de la variable
        - type : type C de la variable
        - expr : expression de définition
        NB: pas besoin de prendre en compte les override ici
        """
        if kind == "Master":
            self.file.write("   static const {type} {name} = {expr};\n".format(
                name=name, type=type, expr=expr))
        elif kind == "Init" or kind == "Depends":
            self.file.write("   static {type} {name};\n".format(
                name=name, type=type))
        else:
            raise RuntimeError("Unknown kind {}".format(kind))

    def write_headers(self):
        sec = self.cg["Headers_header"]
        for h in sec:
            reason = sec[h]
            self.file.write("#include <{}>".format(h))
            if reason:
                self.file.write(" // {}".format(reason))
            self.file.write("\n")
        self.file.write("\n")

    def conclude(self):
        self.file.write("};\n\n")
        self.file.write("typedef Config CG;\n\n")
        self.file.write("#endif // defined F_CONFIG_H\n")
        if self.file is not stdout: self.file.close()


class ConfigSource(object):
    def __init__(self, source, ini):
        self.cg = ini
        self.platforms = [sec.split('_')[1] for sec in self.cg.sections() if 'Override_' in sec]
        self.file = stdout if source == "-" else open(source, 'w')
        self.file.write('#include "config.h"\n')

    def parse(self):
        self.write_headers()
        self.write_ctor()
        self.write_update_params()
        self.write_definitions()

    def write_headers(self):
        sec = self.cg["Headers_source"]
        for h in sec:
            reason = sec[h]
            self.file.write("#include <{}>".format(h))
            if reason:
                self.file.write(" // {}".format(reason))
            self.file.write("\n")
        self.file.write("\n")

    def write_ctor(self):
        self.file.write('Config::Config()\n{\n')
        cur_sec = self.cg["Init"]
        for var in cur_sec:
            overrides = [o for o in self.platforms if var in self.cg["Override_"+o]]
            if not overrides:
                self.add_variable(cur_sec, var)
            else:
                self.add_variable_ctor_override(var,overrides)
        self.file.write("   updateParams();\n")
        self.file.write("}\n\n")

    def write_update_params(self):
        self.file.write("void Config::updateParams()\n{\n")
        cur_sec = self.cg["Depends"]
        for var in cur_sec:
            self.add_variable(cur_sec, var, capitalize=True)
        self.file.write("}\n\n")

    def write_definitions(self):
        for sec in ["Master", "Init", "Depends"]:
            for var in self.cg[sec]:
                type, expr = explode_expr(self.cg[sec][var])
                var = var.upper()
                if sec == "Master":
                    self.file.write("const {type} Config::{name};\n".format(
                        type=type, name=var))
                else:
                    self.file.write("{type} Config::{name};\n".format(
                        type=type, name=var))


    def add_variable(self, section, var, capitalize=False):
        if "=" in section[var]:
            type, expr = explode_expr(section[var])
            expr = self.capitalize(expr)
            var = var.upper()
            self.file.write("   {name} = {expr};\n".format(
                name=var, expr=expr))
        else: # nom = expr
            expr = section[var]
            self.file.write("   {name} = {expr};\n".format(
                name=var.upper(), expr=expr))

    def capitalize(self, expr):
        """
        Transforme : width = 2*height + omega
        en : width = 2*HEIGHT + OMEGA
        """
        expr = expr.upper()
        return re.sub(r'([A-Z_]+) *\(',lambda x: x.group(0).lower(), expr)

    def add_variable_ctor_override(self, var, overrides):
        self.file.write("  #if defined(F_CONFIG_{})\n".format(overrides[0].upper()))
        self.add_variable(self.cg["Override_{}".format(overrides[0])], var)
        for o in overrides[1:]:
            self.file.write("  #elif defined(F_CONFIG_{})\n".format(o.upper()))
            self.add_variable(self.cg["Override_{}".format(o)], var)
        self.file.write("  #else\n")
        self.add_variable(self.cg["Init"], var)
        self.file.write("  #endif\n\n")



def explode_expr(expr):
    """
    Convertit une expression de la forme 'type = expr' en (type, expr)
    """
    type, expr = expr.split(" = ")
    return (type.strip(), expr.strip())

if __name__ == "__main__":
    cg = configparser.ConfigParser()
    cg.optionxform = str
    cg.read(ini_filename)
    header = ConfigHeader(header_filename, cg)
    header.parse()
    source = ConfigSource(source_filename, cg)
    source.parse()
