#! /usr/bin/python
# Filename: assert.py

mylist = ['item']
assert len(mylist) >= 1

mylist.pop()
assert len(mylist) >= 1

# The assert statement should be used judiciously.
# Most of the time, it is better to
# catch exceptions, either handle the problem
# or display an error message to the user
# and then quit.

