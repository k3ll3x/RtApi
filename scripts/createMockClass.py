#!/usr/bin/python3

from create_tree import *

# [private|public] [ref|*] typename

# sub = {
#     # 'string':'newstring',
# }

# tree = tree.replace(sub, regex=True)

cstemplate = """
#include "gmock/gmock.h"

class Mock{classname} : public {classname}{
public:
{functions}
};
"""

# for each class
classes = []
try:
    classes = tree[tree['K'].isin(['class','struct'])]['N'].unique()
except:
    print("No classes or structs here m8")
    sys.exit(-1)

for c in classes:
    ndf = tree[tree['Z'].str.contains(c, na=False)]

    # functions
    functionstmplt = ""
    for index, row in ndf[ndf['K'] == 'function'].iterrows():
        functionstmplt += "\tMOCK_METHOD({0}, {1}, ({2}), (override);\n".format(
            row['t'] + ' ' if row['t'] != '-' else '',
            row['N'],
            row['S']
        )
    
    tmplt = cstemplate.format(classname=c, functions=functionstmplt)

    f = open("{0}/Mock{1}.h".format(gen_path,c),"w")
    f.write(tmplt)
    f.close()