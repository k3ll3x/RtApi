#!/usr/bin/python3
# coding: utf-8

# In[ ]:


import sys
import numpy
import numpy as np
import tensorflow as tf
import keras

from numpy import array
from model import create_model
from keras.utils import np_utils
from keras.layers import Activation
from keras.layers import Embedding
# from keras.utils.np_utils import to_categorical
from keras.utils import to_categorical
from keras import layers
import nltk
nltk.download('punkt')
nltk.download('averaged_perceptron_tagger')
from nltk.tokenize import sent_tokenize, word_tokenize, PunktSentenceTokenizer
from nltk.corpus import stopwords, state_union
import matplotlib.pyplot as plt
from nltk.stem import PorterStemmer

from os import listdir
from os.path import isfile, join

EPOCH = 20
BATCHSZ = 128

def usage():
	print(f"{sys.argv[0]} path_to_dir")

if len(sys.argv) < 2:
	usage()
	sys.exit()

path = sys.argv[1]

#################################################### PARSING ####################################################

files = [f for f in listdir(path) if isfile(join(path, f))]
raw_text = ""

for file in files:
    ## open file
    try:
        raw_text += open(path + file).read()
    except:
        print(f"Error opening {file}")
        # sys.exit()

raw_text = word_tokenize(raw_text.lower())

## sorting for parsing
print("sorting for parsing")
deter = [nltk.pos_tag([i]) for i in raw_text]
all_dets = []
for i in range(len(deter)):
    all_dets += [deter[i][0][1]]


## vocab sorting --> text gen
words = sorted(list(set(raw_text)))
n_words = len(raw_text)
n_vocab = len(words)
print("Total Words: ", n_words)
print("Unique Words: ", n_vocab)

## vocab sorting --> parsing
ind_dets = list(set(all_dets))
n_inds = len(all_dets)
n_individuals = len(ind_dets)
print("-------------------")
print("Total Labels: ", n_inds)
print("Unique Labels: ", n_individuals)



det_li = [nltk.pos_tag([i]) for i in words]
det_list = []
for i in range(len(det_li)):
    det_list += [det_li[i][0][1]]


## sorting outputs --> text gen
word_to_int = dict((c, i) for i, c in enumerate(words))
int_to_word = dict((i, c) for i, c in enumerate(words))

## sorting outputs --> parsing
wow_to_int = dict((c, i) for i, c in enumerate(ind_dets))
int_to_wow = dict((i, c) for i, c in enumerate(ind_dets))


## seq length
seq_length = 22

########################################## PARSING ##################################################

dataX_p = []
dataY_p = []
for i in range(0, n_inds - seq_length, 1):
    seq_in_p = all_dets[i:i + seq_length]
    seq_out_p = all_dets[i + seq_length]
    dataX_p.append([wow_to_int[char] for char in seq_in_p])
    dataY_p.append(wow_to_int[seq_out_p])
n_patterns_p = len(dataX_p)
print("Total Patterns: ", n_patterns_p)

# reshape X to be [samples, time steps, features]
X_p = numpy.reshape(dataX_p, (n_patterns_p, seq_length, 1))

# normalize
X_p = X_p / float(n_individuals)

# one hot encode the output variable
# y_p = np_utils.to_categorical(dataY_p)
y_p = to_categorical(dataY_p)

print("Save numpy arrays, X_p and y_p, for reuse")

f = open("X_p.npy","wb")
np.save(f, X_p)
f.close()

f = open("y_p.npy","wb")
np.save(f, y_p)
f.close()

print("Creating model and callback_list")
filepath="weights-improvement_-{epoch:02d}-{loss:.4f}.hdf5"
new_model, callbacks_list = create_model(X_p, y_p, filepath)

## Training
# new_model.fit(X_p, y_p, epochs=EPOCH, batch_size=BATCHSZ, callbacks=callbacks_list)
new_model.load_weights("weights-improvement_-20-2.2456.hdf5")
score = new_model.evaluate(X_p, y_p, batch_size=BATCHSZ)
print("SCORE: ", score)

########################################## TEXT GEN ##################################################

dataX = []
dataY = []
for i in range(0, n_words - seq_length, 1):
    seq_in = raw_text[i:i + seq_length]
    seq_out = raw_text[i + seq_length]
    dataX.append([word_to_int[char] for char in seq_in])
    dataY.append(word_to_int[seq_out])
n_patterns = len(dataX)


## reshape X to be [samples, time steps, features]
X = numpy.reshape(dataX, (n_patterns, seq_length, 1))

## normalize
X = X / float(n_vocab)

## one hot encode the output variable
# y = np_utils.to_categorical(dataY)
y = to_categorical(dataY)

print("create other model")
filepath2="weights-improvement_2-{epoch:02d}-{loss:.4f}.hdf5"
model, callbacks_list2 = create_model(X_p, y_p, filepath)

## Training
model.fit(X, y, epochs=EPOCH, batch_size=BATCHSZ, callbacks=callbacks_list2)
score = model.evaluate(X, y, batch_size=BATCHSZ)
print("SCORE: ", score)

print("Done training.")

########################################## FULL ASSEMBELED TEXT GEN ##################################################

# seq_length = 500
seq_length = 3000
if len(sys.argv) > 3:
    seq_length = int(sys.argv[2])

# pick a random seed
start_p = numpy.random.randint(0, len(dataX_p)-1)
pattern_p = dataX_p[start_p]
print("Seed:")
print("\"", ' '.join([int_to_wow[value] for value in pattern_p]), "\"")
print("\n")

# pick a random seed
start = numpy.random.randint(0, len(dataX)-1)
pattern = dataX[start]
print("Seed:")
print("\"", ' '.join([int_to_word[value] for value in pattern]), "\"")
print("\n")

newSentence = ""

for i in range(seq_length):
    x_p = numpy.reshape(pattern_p, (1, len(pattern_p), 1))
    x_p = x_p / float(n_individuals)
    prediction_p = new_model.predict(x_p, verbose=0)
    
    x = numpy.reshape(pattern, (1, len(pattern), 1))
    x = x / float(n_vocab)
    prediction = model.predict(x, verbose=0)
    
    index_p = numpy.argmax(prediction_p)
    result_p = int_to_wow[index_p]
    pattern_p.append(index_p)
    pattern_p = pattern_p[1:len(pattern_p)]
    
    for o in range(len(prediction[0])):
        a = wow_to_int[det_list[o]]
        prediction[0][o] = prediction[0][o]*prediction_p[0][a]

    index = numpy.argmax(prediction)
    result = int_to_word[index]+" "
    seq_in = [int_to_word[value] for value in pattern]
    sys.stdout.write(result)
    newSentence += result
    pattern.append(index)
    pattern = pattern[1:len(pattern)]
    
    
print("\n\n\nCleaned: \n")
newBunch = nltk.word_tokenize(newSentence)
newJunk = ""
for i in newBunch:
    if len(i) > 1:
        newJunk += i + " "

# print(newJunk)
f = open("../generated/magic.txt","w")
f.write(newJunk)
f.close()

print("\nDone.")

