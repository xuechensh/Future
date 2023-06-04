import numpy as np   #数据类型默认为64位
import tensorflow as tf         #数据类型默认为32位
import tensorflow.compat.v1 as tf
print("hello")
tf.disable_v2_behavior()
       #数据类型改为32位
data = np.float32(np.load('line_fit_data.npy'))  # 导入100个样本数据
x_data = data[:, :2]   # 样本自变量
y_data = data[:, 2:]   # 样本实际值
'''
定义计算（计算图）
'''
w = tf.Variable(tf.zeros([2, 1]))   #构建变量两行一列a和b
bias = tf.Variable(tf.zeros([1]))    #构建c
y = tf.matmul(x_data, w) + bias   # 构造一个线性模型，矩阵乘法操作，数据和w相乘加上c
loss = tf.reduce_mean(tf.square(y_data - y))  # 定义损失函数，越低越好(均方误差)
 
tf.compat.v1.disable_eager_execution()
optimizer = tf.compat.v1.train.GradientDescentOptimizer(0.5)   # 构建梯度下降法优化器
train = optimizer.minimize(loss)   # 定义训练函数
'''
执行计算（会话中）
'''
# 启动会话
sess = tf.Session()
sess.run(tf.global_variables_initializer())   # 初始化所有变量
# sess.run(y)
for i in range(100):
    print('第', i, '轮训练后的模型损失值：', sess.run(loss))
    sess.run(train)   # 开始训练
 
print(sess.run([w, bias]) )  # y = 0.1*x1 + 0.2*x2 + 0.3
sess.close()
 