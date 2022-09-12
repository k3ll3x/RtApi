#!/usr/bin/python3

# Script meant to run in the docker container

from create_tree import *

# Create Interface
f = open(tmplts_path + "/Interface.h")
itmplt = f.read()
f.close()

for i in modules.keys():
	itmplt = itmplt.replace("$CNAME", i + "Interface")
	# itmplt.replace("$ARGSC", )
	# if '-' in modules[i]['function'].keys():
	# 	modules[i]['function']['-']