import numpy as np
from scipy.misc import imread, imsave, imresize#, imshow
from scipy.spatial.distance import pdist, squareform
import matplotlib.pyplot as plt

img = imread('../openCV/samples/data/fruits.jpg')

print img.dtype, img.shape

img_tinted = img * [1, 0.95, 0.9]

plt.subplot(1, 2, 1)
plt.imshow(img)
plt.subplot(1, 2, 2)
plt.imshow(np.uint8(img_tinted))

img_tinted = imresize(img_tinted, (300, 300))

#imshow(img_tinted)

x = np.array([[0,1], [1,0], [2,0]])
print x

d = squareform(pdist(x, 'euclidean'))
print d

x = np.arange(0, 3 * np.pi, 0.1)
y_sin = np.sin(x)
y_cos = np.cos(x)

# plt.plot(x, y_sin)
# plt.plot(x, y_cos)
# plt.xlabel('x axis label')
# plt.ylabel('y axis label')
# plt.title('Sine and Cosine')
# plt.legend(['Sine', 'Cosine'])

plt.show()


