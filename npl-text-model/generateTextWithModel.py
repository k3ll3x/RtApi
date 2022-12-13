#!/usr/bin/python3
import tensorflow as tf
import sys

n = 100
if len(sys.argv) > 1:
    n = int(sys.argv[1])

model_name = 'one_step'
if len(sys.argv) > 2:
    model_name = sys.argv[2]

one_step_reloaded = tf.saved_model.load(model_name)

states = None
next_char = tf.constant(['Abstract: '])
result = [next_char]

for n in range(n):
  next_char, states = one_step_reloaded.generate_one_step(next_char, states=states)
  result.append(next_char)

print('--------\n\n\n')

print(tf.strings.join(result)[0].numpy().decode("utf-8"))
