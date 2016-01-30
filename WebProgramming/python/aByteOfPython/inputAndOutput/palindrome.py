#! /usr/bin/python
# Filename: palindrome.py

def reverse(text):
    return text[::-1]

def is_palindrome(text):
    text = remove_punctuation(text)
## TODO:
## turn all capital letter to lower-case letter
    return text == reverse(text)

def remove_punctuation(text):
    forbidden = ('!', '?', '.', ':',\
                 ';', ' ', '(', '>'\
                 ')', '[', ']', '<',\
                 "'", '"', '/', ',')

    for i in forbidden:
        if i in text:
            text = text.replace(i, '')
    return text

# something = input("Enter text: ")
something = raw_input("Enter text: ")
if is_palindrome(something):
    print("Yes, it is a palindrome")
else:
    print("No, it is not a palindrome")


