'''
Softmax 回归。计算accuracy。
'''
#from answerMultiLayerPerceptron import buildGraph, lr, wd1, wd2, batchsize
from autograd.BaseGraph import Graph
from autograd.utils import buildgraph
from autograd.BaseNode import *
import mnist
import numpy as np
import pickle
from autograd.utils import PermIterator
from util import setseed
import answerRandomForest
# from PIL import Image

from scipy.ndimage import rotate, shift, zoom

def rotate_image(image, angle):
    return rotate(image, angle, reshape=False)
def shift_image(image, dx, dy):
    return shift(image, (dx, dy), cval=0)
def add_noise(image, mean=0, stddev=10):
    noise = np.random.normal(mean, stddev, image.shape)
    return np.clip(image + noise, 0, 255)

# def shift_image(image, dx, dy):
    # return image.transform((28, 28), Image.AFFINE, (1, 0, dx, 0, 1, dy))
def random_scale_image(image, min_scale, max_scale):
    scale = np.random.uniform(min_scale, max_scale)
    image = image.resize((int(28 * scale), int(28 * scale)))
    return image.resize((28, 28))

def random_scale_image(image, min_scale, max_scale):
    scale_factor = np.random.uniform(min_scale, max_scale)
    scaled_image = zoom(image, scale_factor)
    scaled_image = np.pad(scaled_image, ((0, image.shape[0]), (0, image.shape[1])), mode='constant', constant_values=0)
    # print(scaled_image.shape, scaled_image[:image.shape[0], :image.shape[1]].shape, image.shape)
    return scaled_image[:image.shape[0], :image.shape[1]]
def process_image(image):
    image = image.reshape(28, 28)
    image = rotate_image(image, np.random.uniform(-30, 30))
    image = shift_image(image, np.random.uniform(-3, 3), np.random.uniform(-3, 3))
    # image = add_noise(image)
    image = random_scale_image(image, 0.90, 1.1)
    return image.reshape(-1)
    #print(image.shape)

    # PIL version:
    # image = Image.fromarray(image)
    # image = image.rotate(np.random.uniform(-30, 30))
    # image = shift_image(image, np.random.uniform(-3, 3), np.random.uniform(-3, 3))
    # image = random_scale_image(image, 0.90, 1.1)

    # image = image.resize((28, 28))
    # image = np.array(image)
    # return image.reshape(-1)
    # finish PIL version

# 超参数
# TODO: You can change the hyperparameters here
lr = 1e-2   # 学习率
wd1 = 1e-5  # L1正则化
wd2 = 1e-10  # L2正则化
batchsize = 88

def buildGraph(Y):
    """
    建图
    @param Y: n 样本的label
    @return: Graph类的实例, 建好的图
    """
    # YOUR CODE HERE
    return Graph([
        Linear(mnist.num_feat, 648),
        BatchNorm(indim = 648),
        tanh(),
        Linear(648, 328),
        BatchNorm(indim = 328),
        tanh(),
        Linear(328, 128),
        BatchNorm(indim = 128),
        tanh(),
        Linear(128, mnist.num_class),
        BatchNorm(indim = mnist.num_class),
        tanh(),
        Softmax(),
        Dropout_Corrected(0.0328),
        NLLLoss(Y.shape[0])
                 ])
setseed(0) # 固定随机数种子以提高可复现性

save_path = "model/yt_mlp.npy"
save_path_forest = "model/yt_forest.npy"

X_comb = np.concatenate((mnist.trn_X, mnist.val_X), axis = 0)
Y_comb = np.concatenate((mnist.trn_Y, mnist.val_Y), axis = 0)
np.random.seed(1919810)
shuffle_indices = np.random.permutation(np.arange(X_comb.shape[0]))
X_comb = X_comb[shuffle_indices]
Y_comb = Y_comb[shuffle_indices]
# X_comb = np.array([add_noise(x) for x in X_comb])
#X_comb = add_noise(X_comb)

def discretize(x, bin=64):
    return np.divmod(x, bin)[0]

def build_tree():
    print("BUILDING TREE:")
    partion = int(0.005 * X_comb.shape[0])
    # answerRandomForest.hyperparams["depth"] = 3
    X=X_comb[:partion]
    Y=Y_comb[:partion]
    val_X=X_comb[partion:]
    val_Y=Y_comb[partion:]
    #forest
    #answerRandomForest.hyperparams["gainfunc"] = eval(answerRandomForest.hyperparams["gainfunc"])
    roots = answerRandomForest.buildtrees(discretize(X), Y)
    with open(save_path_forest, "wb") as f:
        pickle.dump(roots, f)
    pred = np.array([answerRandomForest.infertrees(roots, discretize(val_X[i])) for i in range(val_X.shape[0])])
    print("valid acc", np.average(pred==val_Y))



if __name__ == "__main__":
    build_tree()

    X_comb = np.array([t for x in X_comb for t in [process_image(x), process_image(x)]])
    Y_comb = np.array([t for y in Y_comb for t in [y, y]])
    shuffle_indices = np.random.permutation(np.arange(X_comb.shape[0]))
    X_comb = X_comb[shuffle_indices]
    Y_comb = Y_comb[shuffle_indices]
    partion = int(0.2 * X_comb.shape[0])
    X=X_comb[:partion]
    Y=Y_comb[:partion]
    val_X=X_comb[partion:]
    val_Y=Y_comb[partion:]

    graph = buildGraph(Y)
    
    # 训练
    best_train_acc = 0
    dataloader = PermIterator(X.shape[0], batchsize)
    for i in range(1, 45+1):
        hatys = []
        ys = []
        losss = []
        graph.train()
        for perm in dataloader:
            tX = X[perm]
            tY = Y[perm]
            graph[-1].y = tY
            graph.flush()
            pred, loss = graph.forward(tX)[-2:]
            hatys.append(np.argmax(pred, axis=1))
            ys.append(tY)
            graph.backward()
            graph.optimstep(lr, wd1, wd2)
            losss.append(loss)
        loss = np.average(losss)
        acc = np.average(np.concatenate(hatys)==np.concatenate(ys))
        print(f"epoch {i} loss {loss:.3e} acc {acc:.4f}")
        if acc > best_train_acc:
            best_train_acc = acc
            with open(save_path, "wb") as f:
                pickle.dump(graph, f)

    # 测试
    with open(save_path, "rb") as f:
        graph = pickle.load(f)
    graph.eval()
    graph.flush()
    pred = graph.forward(val_X, removelossnode=1)[-1]
    haty = np.argmax(pred, axis=1)
    print("valid acc", np.average(haty==val_Y))

    

    
    

