#!/usr/bin/python3

# tensorflow error: pip install -U tensorflow==2.3.0
from guesslang import Guess

guess = Guess()

def guess_file(filename):
    f = open(filename)
    t = f.read()
    f.close()
    return guess.language_name(t)

def guess(text):
    return guess.language_name(text)
