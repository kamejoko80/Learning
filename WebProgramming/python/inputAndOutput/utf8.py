#! /usr/bin/python
# Filename: utf8.py

# encoding = utf-8
import io

f = io.open("utf8.txt", "wt", encoding = "utf-8")
f.write(u"Imagine non-English language here")
f.close()

text = io.open("utf8.txt", encoding = "utf-8").read()
print(text)


