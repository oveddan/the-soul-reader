import json
import random

words_data=json.load(open('./wordnet.json'))

synset = words_data['synset']
synset_keys = list(synset.keys())
num_words = len(synset_keys)

def random_word_key():
  random_key_index = random.randint(0, num_words)
  return synset_keys[random_key_index]

def get_word(key):
  return synset[key]

def get_random_word_key(key):
  words_for_key = get_word(key)['word']
  return random.choice(words_for_key)

def to_key_word(key, word):
  return key + ':'+word.replace('_', ' ')

def random_words(number):
  words_list = []

  for i in range(0, number):
    words_list.append(random_word())
  
  return words_list

def random_word():
  key = random_word_key()
  word = get_random_word_key(key)
  return to_key_word(key, word)

def similar_word(word_key):
  word = synset[word_key]

  pointers = word['pointer']
  random_pointer = random.choice(pointers)
  key = random_pointer['synset']
  word = get_random_word_key(key)

  return to_key_word(key, word)