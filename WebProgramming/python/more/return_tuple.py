#! /usr/bin/python
# Filename: return_tuple.py

def get_error_details():
    return (2, 'details')

errnum, errstr = get_error_details()
print errnum, errstr

a = 5
b = 8

a, b = b, a

print 'a =', a, ' b =', b
