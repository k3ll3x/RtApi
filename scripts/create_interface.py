#!/usr/bin/python3

# Script meant to run in the docker container

from create_tree import *

# Emscripten template
f = open(tmplts_path + "/emcBind.h")
ctmplt = f.read()
f.close()

#get template functions
ctmplt = re.sub("//(.*)\n", "", ctmplt)
loops = re.findall(r"@.*?@", ctmplt, flags=re.DOTALL)

d = {}

z = {}

classes_names = list(tree[tree['K'] == 'class']['N'])
for i in namespace:
    # if i not in ['','-']:
    z[i] = tree[tree['Z'] == i]
    # tree = tree.drop(z[i].index)

# print(tree)

# z = re.sub("\w+(?s:.*)::", "", tree['Z'])
# functions = tree[(tree['K'] == 'function')]
# functions = tree[(tree['K'] == 'function') & ~(classes.isin(tree['Z']))]

# print(classes)
# print(functions)
# print(loops)

# Template Loops: @name ...
for i in loops:
    k = re.findall("@\w+", i)[-1][1:]

    keys = np.array(re.findall("\$\-\w+\.", i))
    ktmplt = np.array(re.findall("\$\-\w+\.(.+)\n", i))

    lam = lambda x: re.findall("\w+", x)[-1]
    clean_keys = np.vectorize(lam)
    lam = lambda x: re.sub("\$\-\w+\.", "", x)
    clean_tmplt = np.vectorize(lam)

    var = dict(zip(
        clean_keys(keys),
        clean_tmplt(ktmplt)
    ))

    d[k] = var

# print(json.dumps(d, indent=4))

for tk in d.keys():
    ctmplt = re.sub("@.*?@", "${0}".format(tk), ctmplt, flags=re.DOTALL, count=1)

# print(classes_names)
# print(z)

# print(pd.DataFrame(d['function'],index=[0]))


# zname = "Jin::Jan::JinJan"
# zname = "ExampleClass"

# df = z[zname]
# df = df[df['K'] == 'function']
# for i in range(len(df)):
#     tmpt = d['function']['function']
#     for j in df.columns:
#         tmpt = tmpt.replace("${0}".format(j), df[j].iloc[i])
#     txt += tmpt + '\n'

txt = ""
for t in ['function', 'class']:
    for zname in z.keys():
        df = z[zname]
        # df = df[df['K'] == 'function']
        for i in range(len(df)):
            k = df['K'].iloc[i]
            if k not in d[t].keys():
                break
            # zk = df['Z'].iloc[i]
            tmpt = d[t][k]
            for j in df.columns:
                tmpt = tmpt.replace("${0}".format(j), df[j].iloc[i])
            txt += tmpt + '\n'
print(txt)

# function_tmplt = ""
# class_tmplt = ""
# for i in z.keys():
#     zn = re.findall("\w+", i)
#     zn = zn[-1] if len(zn) >= 1 else ""
#     # print(zn)
#     # Classes
#     # print(i)
#     if zn in classes_names:
#         # class constructor
#         print()
#         # print(z[i]['K'] == 'function')
#         # for j in z[i].keys():
#         #     print(z[i][j])
#         # d["class"]["constructor"]
#         # print(c)
#         # for j in z[c].keys():
#             # z[c][j] == 'function' and
#             # d["class"]["constructor"]
#             # print(j)
#             # print()
#             # d["class"]["function"]
#     # functions
#     else:
#         nz = z[i]
#         df = nz[nz['K'] == 'function']
#         tmplt = ""
#         k = nz['K']
#         # print(nz)
#         print(df)
#         for c in nz.columns:
#             # print("d3")
#             # print(nz[c])
#             # k = z[i].iloc[[c]]['K'].iloc[0]
#             tag = "${0}".format(c)
#             if nz[c].empty:
#                 break
#             else:
#                 val = nz[c].iloc[0]
#             # val = z[i].iloc[[i]][c].iloc[0]
#             # print(tag, val, d[k])
#             # print(tag, val)
#             # print(d['function'])
#             # if tag in d[k][k]:# and val not in ['', '-']:
#             #     tmplt += '\t' + re.sub(
#             #         tag.replace("$", "\$"),
#             #         val,
#             #         d[k][k]
#             #     ) + '\n'
#         function_tmplt += tmplt

# ctmplt = re.sub("\$function", function_tmplt, ctmplt)

# print(ctmplt)

# ftmplt = ""
# for i in range(len(functions)):
#     # print(functions.iloc[[i]]['K'].iloc[0])
#     # print(functions.iloc[[i]])
#     ntmplt = ""
#     for c in functions.columns:
#         k = functions.iloc[[i]]['K'].iloc[0]
#         tag = "${0}".format(c)
#         val = functions.iloc[[i]][c].iloc[0]
#         # print(tag, d[k][k])
#         if tag in d[k][k]:# and val not in ['', '-']:
#             ntmplt += '\t' + re.sub(
#                 tag.replace("$", "\$"),
#                 val,
#                 d[k][k]
#             ) + '\n'
#     ftmplt += ntmplt

# ctmplt = re.sub("\$function", ftmplt, ctmplt)
