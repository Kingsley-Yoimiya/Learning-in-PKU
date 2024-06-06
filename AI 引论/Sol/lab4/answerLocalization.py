from typing import List
import numpy as np
from utils import Particle

### 可以在这里写下一些你需要的变量和函数 ###
COLLISION_DISTANCE = 1
MAX_ERROR = 50000

### 可以在这里写下一些你需要的变量和函数 ###

def checkStatus(position, walls):
    for x in walls:
        if ((x - position) ** 2).sum() <= 0.5: 
            return False
    return True

def generNode(walls, rightMost):
    position = np.random.rand(1, 2) * rightMost
    # print("FIND:", position)
    if not checkStatus(position, walls):
        return generNode(walls, rightMost)
    return position

def generate_uniform_particles(walls, N):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    N: int, 采样点数量
    输出：
    particles: List[Particle], 返回在空地上均匀采样出的N个采样点的列表，每个点的权重都是1/N
    """
    all_particles: List[Particle] = []
    ### 你的代码 ###
    rightMost = np.max(walls, axis = 0)
    # print("RIGHTMOST: ", rightmost)
    for _ in range(N):
        position = generNode(walls, rightMost)
        # print("GET: ", position)
        all_particles.append(Particle(position[0][0], position[0][1], np.random.uniform(0, 2 * np.pi), 1 / N))
    # print(all_particles)
    ### 你的代码 ###
    return all_particles

CALCWEIGHT_K = 0.0001

def calculate_particle_weight(estimated, gt):
    """
    输入：
    estimated: np.array, 该采样点的距离传感器数据
    gt: np.array, Pacman实际位置的距离传感器数据
    输出：
    weight, float, 该采样点的权重
    """
    ### 你的代码 ###
    # print("!",np.sqrt(((estimated - gt) ** 2).sum()))
    # print("!!!", np.exp(-CALCWEIGHT_K * np.sqrt(((estimated - gt) ** 2).sum())))
    return np.exp(-CALCWEIGHT_K * np.sqrt(((estimated - gt) ** 2).sum()))
    # print(weight)
    ### 你的代码 ###


def resample_particles(walls, particles: List[Particle]):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    particles: List[Particle], 上一次采样得到的粒子，注意是按权重从大到小排列的
    输出：
    particles: List[Particle], 返回重采样后的N个采样点的列表
    """
    resampled_particles: List[Particle] = []
    ### 你的代码 ###
    resWeight, cnt, tot = 1, 0, len(particles)
    leftMost = np.min(walls, axis = 0)
    rightMost = np.max(walls, axis = 0)
    # print("!" , leftMost, rightMost)
    resWeight = 0
    for x in particles:
        resWeight += x.weight
    # print(resWeight, tot)
    for x in particles:
        x.weight *= 1 / resWeight
    resWeight = 0
    for x in particles:
        resWeight += x.weight
    # print(resWeight, tot)

    resWeight = 0

    for x in particles:
        # print(x.weight)
        resWeight += x.weight
        ncnt = int(np.floor(resWeight * tot))
        for _ in range(ncnt):
            y = x
            y.weight = resWeight / ncnt
            # print(y.position, np.random.normal(0, 1, 2))
            y.position += np.random.normal(0, 0.5, 2)
            y.position[0] = max(leftMost[0], min(y.position[0], rightMost[0]))
            y.position[1] = max(leftMost[1], min(y.position[1], rightMost[1]))
            while not checkStatus(y.position, walls):
                y.position = x.position + np.random.normal(0, 0.5, 2)
                y.position[0] = max(leftMost[0], min(y.position[0], rightMost[0]))
                y.position[1] = max(leftMost[1], min(y.position[1], rightMost[1]))
            y.theta += np.random.normal(0, 0.01)
            resampled_particles.append(y)
            resWeight -= y.weight
        cnt += ncnt
    # print(resWeight, tot - cnt, cnt)
    for _ in range(tot - cnt):
        v = generNode(walls, rightMost)
        resampled_particles.append(Particle(v[0][0], v[0][1], np.random.uniform(0, 2 * np.pi), resWeight / (tot - cnt)))
    ### 你的代码 ###
    return resampled_particles

def apply_state_transition(p: Particle, traveled_distance, dtheta):
    """
    输入：
    p: 采样的粒子
    traveled_distance, dtheta: ground truth的Pacman这一步相对于上一步运动方向改变了dtheta，并移动了traveled_distance的距离
    particle: 按照相同方式进行移动后的粒子
    """
    ### 你的代码 ###
    p.theta += dtheta
    # print([np.cos(p.theta), np.sin(p.theta)])
    # print(traveled_distance)
    p.position += [np.cos(p.theta) * traveled_distance, np.sin(p.theta) * traveled_distance]
    ### 你的代码 ###
    return p

def get_estimate_result(particles: List[Particle]):
    """
    输入：
    particles: List[Particle], 全部采样粒子
    输出：
    final_result: Particle, 最终的猜测结果
    """
    print("RESULT:", max(particles, key = lambda x : x.weight).position)
    return max(particles, key = lambda x : x.weight)