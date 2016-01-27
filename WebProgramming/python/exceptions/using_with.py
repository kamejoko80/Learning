#! /usr/bin/python3
# Filename: using_with.py

with open("poem.txt") as f:
    for line in f:
        print(line, end='')
