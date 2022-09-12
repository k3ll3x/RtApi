#!/usr/bin/python3

# Script meant to run in the docker container

from create_tree import *

# Emscripten template
f = open(tmplts_path + "/emcClass.h")
ctmplt = f.read()
f.close()

print("Shepl")

#get template functions
ctmplt = re.sub("//(.*)\n", "", ctmplt)
class_tmplt = re.findall(r"@(.*)@", ctmplt, flags=re.DOTALL)[-1]
ctmplt =  re.sub(r"@(.*)@", "$MODULES", ctmplt, flags=re.DOTALL)

print(class_tmplt)
print(ctmplt)