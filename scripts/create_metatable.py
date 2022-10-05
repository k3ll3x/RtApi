#!/usr/bin/python3

# Create template for LAak Metatable

from random import randrange

from create_tree import *
sub = {
    'operator [*][=]': '__mul',
    'operator [+][=]': '__add',
    'operator [-][=]': '__sub',
    'operator [=][=]': '__eq',
    'operator /[=]': '__div',
    'operator \[\]': '__newindex',
}

tree = tree.replace(sub, regex=True)

lb = "[-"
rb = "-]"

htemplate = """
#pragma once
#include "LAakTable.h"
#include <memory>
#include "{classname}.h"

class Meta{classname} {lb}
private:
    static std::map<std::string, const std::string> docs = {lb}
{docs}
    {rb}
{functions}
    static int tostring(lua_State* L);
    static int free(lua_State* L);
    static int add(lua_State* L);
    static int sub(lua_State* L);
    static int mul(lua_State* L);
    static int eq(lua_State* L);

    static int doc(lua_State* L){lb}
        std::string who = "doc";
        if(lua_isstring(L, 1)){lb}
            who = luaL_checkstring(L, 1);
        {rb}
        if(!docs.contains(who)){lb}
            return luaL_error(L, "No documentation available");
        {rb}
        if(lua_checkstack(L, 1)){lb}
            lua_pushfstring(L, "%s", docs[who].c_str());
        rightbelseleftb
            return luaL_error(L, LAakTable::nospacestack);
        {rb}
        return 1;
    ]

    static const luaL_Reg methods[] = {lb}
        {lmethods}
        {lb} nullptr, nullptr {rb}
    {rb};

    static const luaL_Reg functions[] = {lb}
{lfunctions}
        {lb} "__gc", free {rb},
        {lb} "__tostring", tostring {rb},
        {lb} nullptr, nullptr {rb}
    {rb};
public:
    Meta{classname}() = delete;
    ~Meta{classname}() = delete;
    static std::string name = "{classname}";
    void reg(lua_State* L){lb}
        LAakTable::register_table(L, name.c_str(), methods, functions);
    {rb}
{rb};
"""

impfunctmplt = """
int Meta{classname}::{function}(lua_State* L){lb}
{rtntype}
    return 1;
{rb}
"""

cctemplate = """
#include "Meta{classname}.h"

{functions}
"""

class_category = ['class','struct']

# for each class
classes = []
try:
    classes = tree[tree['K'].isin(class_category)]['N'].unique()
    # classes = tree['N'].unique()
except:
    # Create namespace functions, if namespace is ""  create name
    print("No classes or structs here m8")
    sys.exit(-1)

rest_df = tree[~tree['K'].isin(class_category)]
rnspace = rest_df['Z']

def make_template(ndf, c):
    if c == "":
        c = "_RTable{0}".format(randrange(999))
    # functions
    functionstmplt = {}
    _impfunctmplt = {}
    docstmpt = {}
    lfunctmplt = {}
    _rtntype = {}
    for index, row in ndf[ndf['K'] == 'function'].iterrows():
        fname = row['N'] if row['t'] != '-' else '__new'
        functionstmplt[row['N']] = fname
        # functionstmplt[fname] = fname
        _impfunctmplt[row['N']] = impfunctmplt.format(
            classname=c,
            function=fname,
            rtntype="$RTNTYPE$",
            rb=rb, lb=lb
        )
        _rtntypetmplt = "\t{0} value = {1}({2});\n"
        _rtntypetmplt_ = "\t{0}* obj = new {0}({1});\n"
        try:
            _rtntype[row['N']] += _rtntypetmplt.format(row['t'],row['N'],row['S']) if row['t'] != '-' else _rtntypetmplt_.format(row['N'], row['S'])
        except:
            _rtntype[row['N']] = _rtntypetmplt.format(row['t'],row['N'],row['S']) if row['t'] != '-' else _rtntypetmplt_.format(row['N'], row['S'])
        _docstmpt = "\t\t{lb}\t{0},\t\t{1} {rb},\n"
        docstmpt[row['N']] = _docstmpt.format(fname, '""', rb=rb, lb=lb)
        _lfunctmplt_ = '\t\t{lb}\t"{0}",\t\t{0} {rb},\n'
        lfunctmplt[row['N']] = _lfunctmplt_.format(fname, rb=rb, lb=lb)

    # avoid empty ones
    if len(functionstmplt) == 0 and len(_impfunctmplt) == 0 and len(docstmpt) == 0 and len(lfunctmplt) == 0 and len(_rtntype) == 0:
        return
    _functionstmplt = ""
    _impfunc = ""
    _docstmplt = ""
    _lfunctmplt = ""
    for i in functionstmplt:
        _impfunc += _impfunctmplt[i].replace("$RTNTYPE$", _rtntype[i] + "$RTNTYPE$")
        _functionstmplt += "\tstatic int {0}(lua_State* L);\n".format(functionstmplt[i])
        _docstmplt += docstmpt[i]
        _lfunctmplt += lfunctmplt[i]
    _impfunc = _impfunc.replace("$RTNTYPE$","")

    tmplt = htemplate.format(classname=c, functions=_functionstmplt, docs=_docstmplt, lfunctions=_lfunctmplt, lmethods="", rb=rb, lb=lb).replace(lb,'{').replace(rb,'}')

    _fname = "{0}/Meta{1}.h".format(gen_path,c)
    f = open(_fname,"w")
    f.write(tmplt)
    f.close()

    tmplt = cctemplate.format(classname=c, functions=_impfunc, rb=rb, lb=lb).replace(lb,'{').replace(rb,'}')

    _fname = "{0}/Meta{1}.cc".format(gen_path,c)
    f = open(_fname,"w")
    f.write(tmplt)
    f.close()

for nsp in rnspace:
    ndf = rest_df[rest_df['Z'].str.contains(nsp)]
    make_template(ndf, nsp)

for c in classes:
# for c in namespace:
    ndf = tree[tree['Z'].str.contains(c)]
    make_template(ndf, c)