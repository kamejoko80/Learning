import numpy as np

a = np.array([1, 2, 3])
print type(a)
print a.shape
print a[0]
a[0] = 5
print a

b = np.array([[1,2,3],[4,5,6]])
print b.shape
print b[1, 0]

a = np.zeros((2, 2))
print a

b = np.ones((1, 2))
print b

c = np.full((2, 2), 7)
print c

d = np.eye(2)
print d

e = np.random.random((2, 2))
print e

a = np.array([[1,2,3,4], [5,6,7,8],[9,10,11,12]])

b = a[:2, 1:3]
print a[0, 1]
b[0, 0] = 77
print a[0, 1]


