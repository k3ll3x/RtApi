#!/usr/bin/python3

# Script meant to be used in Docker container

from create_tree import *


# Emscripten template
f = open(tmplts_path + "/emcClass.h")
ctmplt = f.read()
f.close()

#get template functions
ctmplt = re.sub("//(.*)\n", "", ctmplt)
class_tmplt = re.findall(r"@(.*)@", ctmplt, flags=re.DOTALL)[-1]
ctmplt =  re.sub(r"@(.*)@", "$MODULES", ctmplt, flags=re.DOTALL)

for i in modules.keys():
	constructors = ""
	functions = ""

	if i != '':
		tmp_class_tmplt = class_tmplt.replace("$CNAME", i)

		c = {}
		cmdl = ["$CONST", "$FUNCS"]
		for v in cmdl:
			c[v] = re.findall("\{0}(.*)?\n".format(v), tmp_class_tmplt)[-1].replace(v,'')
			tmp_class_tmplt = re.sub("\{0}(.*)?\n".format(v), v + '\n', tmp_class_tmplt)

		# print(c)
		for j in modules[i].keys():
			if j == "function":
				for j in modules[i]['function'].keys():
					# constructors
					if isinstance(j, int):
						for k in modules[i]['function'][j].keys():
							if '~' not in k:
								constructors += c['$CONST'].replace("$ARGSC", modules[i]['function'][j][k]) + "\n\t"
					# Function
					else:
						for k in modules[i]['function'][j].keys():
							if 'operator' not in k:
								functions += c['$FUNCS'].replace("$FNAME", k) + "\n\t"
		
		tmp_class_tmplt = tmp_class_tmplt.replace("$CONST", constructors)
		tmp_class_tmplt = tmp_class_tmplt.replace("$FUNCS", functions)
		if constructors != '':
			classes += tmp_class_tmplt

print(ctmplt.replace("$MODULES",classes))

# print(txt)
# ch = input(">")
# while(ch != "quit"):
# 	eval(ch)
# 	ch = input(">")

# if '/' in filename:
# 	filename = filename.split('/')[-1]
# filename = gen_path + "/" + filename
# f = open(filename, "w")
# f.write(txt)
# f.close()

# os.popen("cat {0} | uniq > tmpf && mv tmpf {0}".format(filename)).read()
