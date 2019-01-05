import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

img=Image.open('sample.png')
img = np.array(img)
print(img.dtype)
print(img.shape)
print(img[0][0])

plt.imshow(img)
plt.show()