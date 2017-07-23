import tensorflow as tf

n_inputs = 3 #3D inputs
n_hidden = 2 #2D codings
n_outputs = n_inputs
learning_rate = 0.01

X = tf.placeholder(tf.float32, shape=[None, n_inputs])
hidden = tf.layers.dense(X, n_hidden)
outputs = tf.layers.dense(hidden, n_outputs)

reconstruction_loss = tf.reduce_mean(tf.square(outputs - X)) #Mean Squared Error
optimizer = tf.train.AdamOptimizer(learning_rate)
training_op = optimizer.minimize(reconstruction_loss)

init = tf.global_variables_initializer()

X_train, X_test = [...] #load dataset

n_iterations = 1000
codings = hidden #the output of the hidden layer provides the coding
with tf.Session() as sess:
    init.run()
    for iteration in range(n_itereations):
        training_op.run(feed_dict={X: X_train}) #no lables (unsupervised)

    condings_val = codings.eval(feed_dict={X: X_test})
