#!/usr/bin/python3

from create_tree import *

cctemplate = """
class {classname} [
    //members
{members}

    //functions
{functions}
];
"""

# for each class
classes = []
try:
    classes = tree[tree['K'].isin(['class','struct'])]['N'].unique()
except:
    # Create namespace functions, if namespace is ""  create name
    print("No classes or structs here m8")
    sys.exit(-1)

for c in classes:
    # ndf = tree[tree['Z'].str.contains(c, na=False)]
    ndf = tree[tree['Z'] == c]
    
    # members
    memberstmplt = ""
    for index, row in ndf[ndf['K'] == 'member'].iterrows():
        memberstmplt += "\t{0}{1} {2};\n".format(
            row['a'] + ':\t' if row['a'] != '-' else '',
            row['t'], row['N']
        )

    # functions
    functionstmplt = ""
    for index, row in ndf[ndf['K'] == 'function'].iterrows():
        functionstmplt += "\t{0}{1}{2}({3});\n".format(
            row['a'] + ':\t' if row['a'] != '-' else '',
            row['t'] + ' ' if row['t'] != '-' else '',
            row['N'],
            row['S']
        )
    
    # tmplt = ""
    # try:
    tmplt = cctemplate.format(classname=c, members=memberstmplt, functions=functionstmplt).replace('[','{').replace(']','}')
    # tmplt = cctemplate.format(classname="c", members="memberstmplt", functions="functionstmplt")
    # except:
    #     continue
        # print(c)
    #     print(memberstmplt)
    #     print(functionstmplt)    

    _fname = "{0}/{1}.h".format(gen_path,c)
    f = open(_fname,"w")
    f.write(tmplt)
    f.close()