#!/usr/bin/python3

# Script meant to be used in Docker container

from create_tree import *

# Emscripten template
f = open(tmplts_path + "/emcFunctions.h")
ctmplt = f.read()
f.close()

# remove comments
ctmplt = re.sub("//(.*)\n", "", ctmplt)
ctmplt = re.sub("/\*(.*)?\*/", "", ctmplt, flags=re.DOTALL)

v = "$FUNCS"
funcs = re.findall("\{0}(.*)?\n".format(v), ctmplt)[-1].replace(v,'')
ctmplt = re.sub("\{0}(.*)?\n".format(v), v + '\n', ctmplt)

func_txt = ""
for i in modules.keys():
	for j in find_key(i, 'function'):
		func_txt += funcs.replace("$FNAME", n) + '\n'

print(ctmplt.replace(v, func_txt))