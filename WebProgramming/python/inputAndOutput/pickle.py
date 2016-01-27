#! /usr/bin/python3
# Filename: pickle.py

## import pickle
import cPickle as p

# The name of the file where we will store the object
shoplistfile = 'shoplist.data'
# The list of things to buy
shoplist = ['apple', 'mango', 'carrot']

# Write to the file
f = open(shoplistfile, 'wb')
# Dump the object to a file
## pickle.dump(shoplist, f)
p.dump(shoplist, f)
f.close()

# Destroy the shoplist variable
del shoplist

# Read back from the storage
f = open(shoplistfile, 'rb')
# Load the object from the file
## storelist = pickle.load(f)
storelist = p.load(f)
print(storelist)


