#! /usr/bin/python
# Filename: func_receiving_tuples_and_dic.py

## receiving parameters to a function
## as a tuple or a dictionary
## using the * or ** prefix respectively.

def powersum(power, *args):
    '''Return the sum of each argument
        raised to the specified power.'''
    total = 0
    for i in args:
        total += pow(i, power)
    return total

print powersum(2, 3, 4)


