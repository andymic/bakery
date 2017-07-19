import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import load_sample_image

# Load sample images
china = load_sample_image('china.jpg')
flower = load_sample_image('flower.jpg')
dataset = np.array([china, flower], dtype=np.float32)
batch_size, height, width, channels = dataset.shape
print(dataset.shape)

# Create 2filters
filters = np.zeros(shape=(7,7, channels, 2), dtype=np.float32)
filters[:, 3, :, 0] = 1 # horizontal
filters[3, :, :, 1] = 1 # vertical

# Create a graph with input X plus a convolutional layer applying the 2 filters
X = tf.placeholder(shape=(None, height, width, channels), dtype=tf.float32)
convolution = tf.nn.conv2d(X, filters, strides=[1,2,2,1], padding='SAME')
# Letting tensorflow create the kernels
conv = tf.layers.conv2d(X, filters=2, kernel_size=7, strides=[2,2], padding='SAME')
init = tf.global_variables_initializer()
with tf.Session() as sess:
    init.run()
    output = sess.run(conv, feed_dict={X:dataset})

for i in range(2):
    plt.imshow(output[i, :, :, 1], cmap='gray') # plot images 2nd feature map
    plt.show()
