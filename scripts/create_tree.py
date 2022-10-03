#!/usr/bin/python3

# Script meant to be used in Docker container

# Creates Tree from C++ File

import sys, os, re, json, collections.abc
import numpy as np
import pandas as pd

path = "/home/devu/repo"

gen_path = path + "/generated"
# tmplts_path = path + "/templates"

def usage():
	print("{0} cppfilename".format(sys.argv[0]))

def cleanf_args(x):
    t = re.sub(r"[(]","",x, count=1)
    l = re.findall(r"\)(.*)",t)
    if len(l) >= 1:
        t = re.sub(l[-1],'',t)
    return t[:-1] if len(t) >= 1 and ')' == t[-1] else t

if len(sys.argv) < 2:
	usage()
	sys.exit(-1)

_filename = sys.argv[1]
filename = _filename

filename = filename.split('/')[-1]
# ufilename = ''.join(filename.split('.')) if '.' in filename else filename
ufilename = filename.split('.')[0] if '.' in filename else filename
filename = gen_path + "/tmp.{0}".format(filename)

#Preprocess file
os.popen("g++ -E -traditional-cpp -fpreprocessed {0} > {1}".format(_filename, filename)).read()

f = open(filename, encoding='utf-8', errors='ignore')
txt = f.read()
f.close()

# columns = ['Z','K','N','t','S','a','i','m','C']
columns = ['N', 'F', 'P', 'C', 'E', 'K', 'R', 'S', 'T', 'Z', 'a', 'e', 'f', 'i', 'k', 'l', 'm', 'n', 'o', 'p', 'r', 's', 't', 'x', 'z']

fmt = ""
for i in columns:
    fmt += "'{0}':'%{0}'{1}".format(i, ',' if i != columns[-1] else '')

cmd = 'ctags -x --_xformat="{0}" {1}'.format(fmt,filename)

txt = os.popen(cmd).read()

raw_modules = []
modules = {}
l = txt.split('\n')

constructors_counter = {}

df = pd.DataFrame(columns=columns)

for i in l:
    if i != '':
        i = '{' + i + '}'
        m = eval(i)
        raw_modules.append(m)
        df = pd.concat([df, pd.DataFrame(m, index=[0])], ignore_index=True)

f = open(gen_path + "/atr." + ufilename + ".json", "w")
f.write(json.dumps(raw_modules, indent=4))
f.close()

# df['t'] = df['t'].apply(lambda x: re.sub("typename:","",x))
df['t'] = df['t'].str.replace("typename:","", regex=True)
df['S'] = df['S'].apply(cleanf_args)

df = df[df['N'] != "class"]
namespace = df['Z'].unique()

tree = pd.DataFrame(columns=columns)

for c in namespace:
	tree = pd.concat([tree, df[df['Z'] == c]], ignore_index=True)

# if file exists, open it, append dataframes and save (Solution for header and cpp)
dffile = gen_path + "/df." + ufilename + ".xls"
if os.path.exists(dffile):
    ndf = pd.read_excel(dffile)
    tree = pd.concat([tree, ndf], ignore_index=True)[columns]

#remove in Z (Scope) extra namespace
tree = tree.replace(np.nan, '', regex=True)
tree.to_excel(dffile, engine="openpyxl")