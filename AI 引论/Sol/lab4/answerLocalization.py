from typing import List
import numpy as np
from utils import Particle

### 可以在这里写下一些你需要的变量和函数 ###
COLLISION_DISTANCE = 0.75

### 可以在这里写下一些你需要的变量和函数 ###

globalWalls = []
particleNum = 0
scope = np.array([10, 10])

def checkCollision(pos, walls):
    t = [int(pos[0] + 0.5), int(pos[1] + 0.5)]
    for p in walls:
        if np.array_equal(p, t):
            return True
    return False

def generNode(walls):
    while True:
        pos = [np.random.uniform(0, scope[0]), np.random.uniform(0, scope[1])]
        if not checkCollision(pos, walls):
            return pos

def generate_uniform_particles(walls, N):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    N: int, 采样点数量
    输出：
    particles: List[Particle], 返回在空地上均匀采样出的N个采样点的列表，每个点的权重都是1/N
    """
    all_particles: List[Particle] = []
    for _ in range(N):
        all_particles.append(Particle(1.0, 1.0, 1.0, 0.0))
    
    global globalWalls, particleNum, scope
    globalWalls = walls
    particleNum = N
    scope = walls.max(axis = 0)

    for _ in range(N):
        pos =  generNode(walls)
        all_particles[_] = Particle(pos[0], pos[1], np.random.uniform(-np.pi, np.pi), 1.0 / N)

    return all_particles


def calculate_particle_weight(estimated, gt):
    """
    输入：
    estimated: np.array, 该采样点的距离传感器数据
    gt: np.array, Pacman实际位置的距离传感器数据
    输出：
    weight, float, 该采样点的权重
    """
    weight = 1.0
    dist = np.linalg.norm(gt - estimated, ord = 1)
    weight = np.exp(-dist * 0.12)
    # print(gt, estimated)
    # print(dist, weight)
    return weight


def resample_particles(walls, particles: List[Particle]):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    particles: List[Particle], 上一次采样得到的粒子，注意是按权重从大到小排列的
    输出：
    particles: List[Particle], 返回重采样后的N个采样点的列表
    """
    resampled_particles: List[Particle] = []
    indices = []
    
    for x in particles:
        for _ in range(int(particleNum * x.weight * 1.03)):
            if len(indices) >= particleNum:
                break
            indices.append(x)

    # np.random.choice(size = int(particleNum * t), p = probability, a = particles).tolist()
    while len(indices) < particleNum:
        pos = generNode(walls)
        indices.append(Particle(pos[0], pos[1], np.random.uniform(-np.pi, np.pi), 1 / particleNum))
    # print([p.position for p in indices])
    for particle in indices:
        # particle.theta += np.random.normal(0, 0.001)
        # particle.position += np.random.normal(0, 0.01, 2)
        # if checkCollision(particle.position, walls) or particle.position[0] < 0 or particle.position[1] < 0 or particle.position[0] > scope[0] or particle.position[1] > scope[1]:
        #     pos = generNode(walls)
        #     particle.position = pos
        #     particle.theta = np.random.uniform(0, 2 * np.pi)
        #     particle.weight = 1.0
        resampled_particles.append(particle)
    # print([x.position for x in resampled_particles])
    return resampled_particles

def apply_state_transition(p: Particle, traveled_distance, dtheta):
    """
    输入：
    p: 采样的粒子
    traveled_distance, dtheta: ground truth的Pacman这一步相对于上一步运动方向改变了dtheta，并移动了traveled_distance的距离
    particle: 按照相同方式进行移动后的粒子
    """
    # print(dtheta, traveled_distance)

    p.theta += dtheta + np.random.normal(0, 0.009)
    p.theta %= np.pi * 2
    # traveled_distance += np.random.normal(0, 0.1)
    p.position += np.array([np.cos(p.theta) * traveled_distance, np.sin(p.theta) * traveled_distance])
    p.position += np.random.normal(0, 0.09, 2)
    return p
    if checkCollision(p.position) or p.position[0] < 0 or p.position[1] < 0 or p.position[0] > scope[0] or p.position[1] > scope[1]:
        pos = generNode()
        p.position = pos
        p.theta = np.random.uniform(0, 2 * np.pi)
        p.weight = 1.0
    return p

def get_estimate_result(particles: List[Particle]):
    """
    输入：
    particles: List[Particle], 全部采样粒子
    输出：
    final_result: Particle, 最终的猜测结果
    """
    final_result = Particle()
    ### 你的代码 ###
    final_result = particles[0]
    ### 你的代码 ###
    return final_result