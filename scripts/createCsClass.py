#!/usr/bin/python3

from create_tree import *

# [private|public] [ref|*] typename

sub = {
    '&': 'ref',
    '*': 'ref',
    'std::atomic':'volatile',
    'std::string': 'String',
    'float':'double',
    'uint64_t': 'UInt64',
    'uint32_t': 'UInt32',
    'uint16_t': 'UInt16',
    'int64_t': 'Int64',
    'int32_t': 'Int32',
    'int16_t': 'Int16'
}

tree = tree.replace(sub, regex=True)

cstemplate = """
class {classname} {
    //members
{members}

    //functions
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
    
    # members
    memberstmplt = ""
    for index, row in ndf[ndf['K'] == 'member'].iterrows():
        memberstmplt += "\t{0}{1} {2};\n".format(
            row['a'] + ' ' if row['a'] != '-' else '',
            row['t'], row['N']
        )

    # functions
    functionstmplt = ""
    for index, row in ndf[ndf['K'] == 'function'].iterrows():
        functionstmplt += "\t{0}{1}{2}({3});\n".format(
            row['a'] + ' ' if row['a'] != '-' else '',
            row['t'] + ' ' if row['t'] != '-' else '',
            row['N'],
            row['S']
        )
    
    tmplt = cstemplate.format(classname=c, members=memberstmplt, functions=functionstmplt)

    f = open("{0}/{1}.cs".format(gen_path,c),"w")
    f.write(tmplt)
    f.close()