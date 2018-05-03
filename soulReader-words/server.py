from flask import Flask
import words

app = Flask(__name__)

@app.route("/random_words")
def random_words():
  random_words = words.random_words(3)

  return ",".join(random_words)

@app.route("/random_word")
def random_word():
  return words.random_word()



@app.route("/similar_word/<word_key>")
def similar_word(word_key):
  word = words.similar_word(word_key)   

  return word

