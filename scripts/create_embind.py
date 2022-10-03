#!/usr/bin/python3

from create_tree import *

functiontmplt = """
function("{name}", &{namespace}::{name});
"""

constructortmplt = """
smart_ptr_constructor("{name}", &std::make_shared<{name}>)
"""

classtmplt = """
class_<{name}>("{name}")
.{functiontmplt}
//.property("$N", &$Z::get$N, &$Z::set$N);
"""

template = """
#pragma once
#include <memory>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(customlibrary) [
    {functiontmplt}
    {classestmplt}
]
"""

# for each class
classes = []
try:
    classes = tree[tree['K'].isin(['class','struct'])]['N'].unique()
except:
    # Create namespace functions, if namespace is ""  create name
    print("No classes or structs here m8")
    sys.exit(-1)

classestmplt = ""
for c in classes:
    # ndf = tree[tree['Z'].str.contains(c, na=False)]
    ndf = tree[tree['Z'] == c]

    # functions
    _functionstmplt = ""
    for index, row in ndf[ndf['K'] == 'function'].iterrows():
        if row['N'] not in row['Z'] or row['N'] == row['Z']:
            _functionstmplt += functiontmplt.format(
                name=row['N'],
                namespace=row['Z']
            )
        else:
            _functionstmplt += constructortmplt.format(
                name=row['N'],
                namespace=row['Z']
            )
    
    classestmplt += classtmplt.format(name=c, functiontmplt=_functionstmplt).replace('[','{').replace(']','}')  

tmplt = template.format(classestmplt=classestmplt, functiontmplt='')

_fname = "{0}/{1}.h".format(gen_path,"emBinding.h")
f = open(_fname,"w")
f.write(tmplt)
f.close()