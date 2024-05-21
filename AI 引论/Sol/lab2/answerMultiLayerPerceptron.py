import mnist
from copy import deepcopy
from typing import List
from autograd.BaseGraph import Graph
from autograd.utils import buildgraph
from autograd.BaseNode import *

# 超参数
# TODO: You can change the hyperparameters here
lr = 1e-3   # 学习率
wd1 = 1e-3  # L1正则化
wd2 = 1e-8  # L2正则化
batchsize = 128

def buildGraph(Y):
    """
    建图
    @param Y: n 样本的label
    @return: Graph类的实例, 建好的图
    """
    # YOUR CODE HERE
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 198),
        tanh(),
        Linear(198, mnist.num_class),
        tanh(),
        Softmax(),
        Dropout(0.030),
        NLLLoss(Y.shape[0])
                 ])
    return Graph([
        Linear(mnist.num_feat, 648),
        relu(),
        Linear(648, 328),
        relu(),
        Linear(328, 68),
        relu(),
        Linear(68, 10),
        Softmax(),
 #       Dropout(0.648),
        CrossEntropyLoss(Y.shape[0])
                 ])
    return Graph([
        Linear(mnist.num_feat, 648),
        relu(),
        Linear(648, 328),
        relu(),
        Linear(328, 128),
        relu(),
        Linear(128, 68),
        relu(),
        Softmax(),
        Dropout(0.448),
        CrossEntropyLoss(Y.shape[0])
                 ])
    
    # SCORE: 0.7899, 96
    return Graph([
        Linear(mnist.num_feat, 256),
        relu(),
        Linear(256, 128),
        Softmax(),
        Dropout(0.8),
        CrossEntropyLoss(Y.shape[0])
                 ])
    # SCORE: 0.8182 97(玩原神玩的)
    return Graph([
        Linear(mnist.num_feat, 648),
        relu(),
        Linear(648, 328),
        relu(),
        Softmax(),
        Dropout(0.618),
        CrossEntropyLoss(Y.shape[0])
                 ])
    # SCORE: 0.8330 97(启动！)
    return Graph([
        Linear(mnist.num_feat, 648),
        relu(),
        Linear(648, 328),
        relu(),
        Softmax(),
        Dropout(0.648),
        CrossEntropyLoss(Y.shape[0])
                 ])
    # SCORE: 0.8467 98(RUN!)
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 128),
        tanh(),
        Linear(128, mnist.num_class),
        Softmax(),
        Dropout(0.068),
        CrossEntropyLoss(Y.shape[0])
                 ])
    # SCORE 0.834159
    lr = 5e-4   # 学习率
    wd1 = 1e-3  # L1正则化
    wd2 = 1e-10  # L2正则化
    batchsize = 128
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 128),
        tanh(),
        Linear(128, mnist.num_class),
        tanh(),
        Softmax(),
        Dropout(0.032),
        NLLLoss(Y.shape[0])
                 ])
    # SCORE: 0.8556086 98
    lr = 7e-4   # 学习率
    wd1 = 1e-3  # L1正则化
    wd2 = 2e-12  # L2正则化
    batchsize = 128
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 128),
        tanh(),
        Linear(128, mnist.num_class),
        tanh(),
        Softmax(),
        Dropout(0.032),
        NLLLoss(Y.shape[0])
                 ])
    # SCORE: 0.8637 99
    lr = 9e-4   # 学习率
    wd1 = 1e-3  # L1正则化
    wd2 = 2e-12  # L2正则化
    batchsize = 128
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 128),
        tanh(),
        Linear(128, mnist.num_class),
        tanh(),
        Softmax(),
        Dropout(0.068),
        NLLLoss(Y.shape[0])
                 ])
    # SCORE: 0.8711 100
    lr = 1e-3   # 学习率
    wd1 = 1e-3  # L1正则化
    wd2 = 1e-12  # L2正则化
    batchsize = 128
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 128),
        tanh(),
        Linear(128, mnist.num_class),
        tanh(),
        Softmax(),
        Dropout(0.030),
        NLLLoss(Y.shape[0])
                 ])
    # SCORE: 0.87137 100
    lr = 1e-3   # 学习率
    wd1 = 1e-3  # L1正则化
    wd2 = 1e-8  # L2正则化
    batchsize = 128
    return Graph([
        Linear(mnist.num_feat, 328),
        tanh(),
        Linear(328, 198),
        tanh(),
        Linear(198, mnist.num_class),
        tanh(),
        Softmax(),
        Dropout(0.030),
        NLLLoss(Y.shape[0])
                 ])