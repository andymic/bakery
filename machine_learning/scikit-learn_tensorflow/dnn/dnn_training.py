import tensorflow as tf
n_inputs = 28*28
n_hidden1 = 300
n_hidden2 = 100
n_outputs = 10

X = tf.placeholder(tf.float32, shape=(None, n_inputs), name='X')
y = tf.placeholder(tf.int64, shape=(None), name='y')

# def neuron_layer(X, n_neurons, name, activation=None):
#     #name scope that will contain the computation nodes
#     with tf.name_scope(name):
#         #getting the matrix's shape by looking at the second dimension - the first is for instances
#         n_inputs = int(X.get_shape()[1]) 
#         stddev = 2/np.sqrt(n_inputs)
#         '''
#             creates the weights between each input and each neuron. It will be initialized randomly using
#             a truncated Gaussian distribution and the standard deviation that helps the algorithm converge 
#             much faster
#         '''
        
#         init = tf.truncated_normal((n_inputs, n_neurons), stddev=stddev)
#         W = tf.Variable(init, name='kernel')
#         # one bias param per neuron initialized to 0
#         b = tf.Variable(tf.zeros([n_neurons]), name='bias')
#         #subgraph to compute
#         Z = tf.matmul(X, W) + b
#         if activation is not None:
#             return activation(Z)
#         else:
#             return Z
        
with tf.name_scope('dnn'):
    hidden1 = tf.layers.dense(X, n_hidden1, name='hidden1', activation=tf.nn.relu)
    hidden2 = tf.layers.dense(hidden1, n_hidden2, name='hidden2', activation=tf.nn.relu)
    #output layer
    logits = tf.layers.dense(hidden2, n_outputs, name='outputs')

with tf.name_scope('loss'):
    xentropy = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=y, logits=logits)
    loss = tf.reduce_mean(xentropy, name='loss')
        
learning_rate = 0.01
with tf.name_scope('train'):
    optimizer = tf.train.GradientDescentOptimizer(learning_rate)
    training_op = optimizer.minimize(loss)
    
with tf.name_scope('eval'):
    correct = tf.nn.in_top_k(logits, y, 1)
    accuracy = tf.reduce_mean(tf.cast(correct, tf.float32))
    
init = tf.global_variables_initializer()
saver = tf.train.Saver()

from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('/tmp/data/')
n_epochs = 40
batch_size = 50

with tf.Session() as sess:
    init.run()
    for epoch in range(n_epochs):
        for iteration in range(mnist.train.num_examples // batch_size):
            X_batch, y_batch = mnist.train.next_batch(batch_size)
            sess.run(training_op, feed_dict={X: X_batch, y:y_batch})
        acc_train = accuracy.eval(feed_dict={X: X_batch, y:y_batch})
        acc_test = accuracy.eval(feed_dict={X: mnist.test.images, y:mnist.test.labels})
        print(epoch, 'Train accuracy: ', acc_train, 'Test accuracy: ', acc_test)
        
    saver_path = saver.save(sess, './my_model_final.ckpt')






