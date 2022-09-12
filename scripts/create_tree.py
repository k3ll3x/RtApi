#!/usr/bin/python3

# Script meant to be used in Docker container

# Creates Tree from C++ File

import sys, os, re, json, collections.abc
import numpy as np
import pandas as pd

path = "/src"

gen_path = path + "/generated"
tmplts_path = path + "/templates"

ctags_fmt_d = "######"

def usage():
	print("{0} cppfilename".format(sys.argv[0]))

def cleanf_args(x):
    t = re.sub(r"[(]","",x, count=1)
    return t[:-1] if len(t) >= 1 and ')' == t[-1] else t

if len(sys.argv) < 2:
	usage()
	sys.exit(1)

_filename = sys.argv[1]
filename = _filename

filename = filename.split('/')[-1]
ufilename = filename.split('.')[0] if '.' in filename else filename
filename = gen_path + "/tmp.{0}".format(filename)

#Preprocess file
os.popen("g++ -E -traditional-cpp -fpreprocessed {0} > {1}".format(_filename, filename)).read()

f = open(filename, encoding='utf-8', errors='ignore')
txt = f.read()
f.close()

columns = ['Z','K','N','t','S']
fmt = "%Z{0}%K{0}%N{0}%t{0}%S".format(ctags_fmt_d)

fmta = re.findall("\w", fmt)
# print(fmta)

cmd = 'ctags -x --_xformat="{0}" {1}'.format(fmt,filename)
# print(cmd)

txt = os.popen(cmd).read()

raw_modules = []
modules = {}
# txt = re.sub("~", "", txt)
l = txt.split('\n')

constructors_counter = {}

df = pd.DataFrame(columns=columns)

for i in l:
    if i != '':
        a = i.split(ctags_fmt_d)
        m = dict(zip(fmta, a))
        raw_modules.append(m)
        df = pd.concat([df, pd.DataFrame(m, index=[0])], ignore_index=True)

f = open(gen_path + "/atr." + ufilename + ".json", "w")
f.write(json.dumps(raw_modules, indent=4))
f.close()

df['t'] = df['t'].apply(lambda x: re.sub("typename:","",x))
df['S'] = df['S'].apply(cleanf_args)
# df['S'] = df['S'].apply(lambda x: re.sub(r"[(].*?(?s:.*)[)]","",x, count=1))
# df['S'] = df['S'].apply(lambda x: re.sub(r".*?(?s:.*)[)]","",x))
# df[df['K'] == 'function'].apply(lambda x: )
# print(df[df['Z'] == "ExampleClass"])

# df = df[df['K']=='member']

# df.to_excel(gen_path + "/df." + ufilename + ".xls", engine="openpyxl")

df = df[df['N'] != "class"]
namespace = df['Z'].unique()

tree = pd.DataFrame(columns=columns)
# tree = []

for c in namespace:
	tree = pd.concat([tree, df[df['Z'] == c]], ignore_index=True)
	# tree.append(df[df['Z'] == c])

tree.to_excel(gen_path + "/df." + ufilename + ".xls", engine="openpyxl")

# while True:
# 	ch = input()
# 	print(eval(ch))
