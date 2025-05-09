import numpy as np
from typing import List
from utils import TreeNode
from simuScene import PlanningMap


### 定义一些你需要的变量和函数 ###
STEP_DISTANCE = 1.1
REDUCE_STEP_DISTANCE = 2
FULL_STEP_DISTANCE = STEP_DISTANCE
MIN_STEP_DISTANCE = 0.4
TARGET_THREHOLD = 1.5
WALK_THREHOLD = 0.5
TRY_LIM = 4
### 定义一些你需要的变量和函数 ###


class RRT:
    def __init__(self, walls) -> None:
        """
        输入包括地图信息，你需要按顺序吃掉的一列事物位置 
        注意：只有按顺序吃掉上一个食物之后才能吃下一个食物，在吃掉上一个食物之前Pacman经过之后的食物也不会被吃掉
        """
        self.map = PlanningMap(walls)
        self.walls = walls
        
        self.scoper = walls.max(axis = 0)
        self.scopel = walls.min(axis = 0)
        # print(self.scopel, self.scoper)
        self.path = None
        
        
    def find_path(self, current_position, next_food):
        """
        在程序初始化时，以及每当 pacman 吃到一个食物时，主程序会调用此函数
        current_position: pacman 当前的仿真位置
        next_food: 下一个食物的位置
        
        本函数的默认实现是调用 build_tree，并记录生成的 path 信息。你可以在此函数增加其他需要的功能
        """
        
        ### 你的代码 ###      

        ### 你的代码 ###
        # 如有必要，此行可删除
        # print("finding path", current_position, next_food)
        self.path = self.build_tree(current_position, next_food)
        self.pathPosition = 0
        self.pathCnt = 0
        # print(self.path)
        # print([self.map.checkline(self.path[i].tolist(), self.path[i+1].tolist()) for i in range(len(self.path)-1)])
        
        
    def get_target(self, current_position, current_velocity):
        """
        主程序将在每个仿真步内调用此函数，并用返回的位置计算 PD 控制力来驱动 pacman 移动
        current_position: pacman 当前的仿真位置
        current_velocity: pacman 当前的仿真速度
        一种可能的实现策略是，仅作为参考：
        （1）记录该函数的调用次数
        （2）假设当前 path 中每个节点需要作为目标 n 次
        （3）记录目前已经执行到当前 path 的第几个节点，以及它的执行次数，如果超过 n，则将目标改为下一节点
        
        你也可以考虑根据当前位置与 path 中节点位置的距离来决定如何选择 target
        
        同时需要注意，仿真中的 pacman 并不能准确到达 path 中的节点。你可能需要考虑在什么情况下重新规划 path
        """
        target_pose = np.zeros_like(current_position)
        self.pathCnt += 1
        # print(len(self.path), self.pathPosition, self.pathCnt)
        
        if self.pathPosition >= len(self.path):
            # print("REGENERATING")
            self.find_path(current_position, self.path[-1])
            # print("REGENER RESULT:", self.path)
        if self.pathCnt > TRY_LIM or np.linalg.norm(current_position - self.path[self.pathPosition]) < WALK_THREHOLD:
            self.pathPosition += 1
            self.pathCnt = 0
        if self.pathPosition >= len(self.path):
            # print("REGENERATING")
            self.find_path(current_position, self.path[-1])
            # print("REGENER RESULT:", self.path)
        # print(current_position, len(self.path), self.pathPosition, self.pathCnt, self.path[self.pathPosition])
        target_pose = self.path[self.pathPosition]
        return target_pose
        
    ### 以下是RRT中一些可能用到的函数框架，全部可以修改，当然你也可以自己实现 ###
    def build_tree(self, start, goal):
        """
        实现你的快速探索搜索树，输入为当前目标食物的编号，规划从 start 位置食物到 goal 位置的路径
        返回一个包含坐标的列表，为这条路径上的pd targets
        你可以调用find_nearest_point和connect_a_to_b两个函数
        另外self.map的checkoccupy和checkline也可能会需要，可以参考simuScene.py中的PlanningMap类查看它们的用法
        """
        global STEP_DISTANCE
        path = []
        graph1: List[TreeNode] = []
        graph2: List[TreeNode] = []
        graph1.append(TreeNode(-1, start[0], start[1]))
        graph2.append(TreeNode(-1, goal[0], goal[1]))
        def gener(id1, id2):
            while graph1[id1].parent_idx != -1:
                path.append(graph1[id1].pos)
                id1 = graph1[id1].parent_idx
            path.reverse()
            while graph2[id2].parent_idx != -1:
                id2 = graph2[id2].parent_idx
                path.append(graph2[id2].pos)
            path.append(goal)
            return path
        while True:
            t = np.random.rand(2) * (self.scoper - self.scopel) + self.scopel
            def addNode(t, graph) :
                nearestId, _ = self.find_nearest_point(t, graph)
                is_empty, newpoint = self.connect_a_to_b(graph[nearestId].pos, t)
                if is_empty:
                    graph.append(TreeNode(nearestId, newpoint[0], newpoint[1]))
                    return newpoint, nearestId
                return None, -1
            v1, id1 = addNode(t, graph1)
            v2, id2 = addNode(t, graph2)
            # print(v1, id1, v2, id2)
            if id1 != -1 and id2 != -1:
                if np.linalg.norm(v1 - v2) < TARGET_THREHOLD:
                    return gener(id1, id2)
            

    @staticmethod
    def find_nearest_point(point, graph):
        """
        找到图中离目标位置最近的节点，返回该节点的编号和到目标位置距离、
        输入：
        point：维度为(2,)的np.array, 目标位置
        graph: List[TreeNode]节点列表
        输出：
        nearest_idx, nearest_distance 离目标位置距离最近节点的编号和距离
        """
        nearest_idx = -1
        nearest_distance = 10000000.
        for _ in range(len(graph)):
            dist = np.linalg.norm(graph[_].pos - point)
            if dist < nearest_distance:
                nearest_distance = dist
                nearest_idx = _
        return nearest_idx, nearest_distance
    
    def connect_a_to_b(self, point_a, point_b):
        """
        以A点为起点，沿着A到B的方向前进STEP_DISTANCE的距离，并用self.map.checkline函数检查这段路程是否可以通过
        输入：
        point_a, point_b: 维度为(2,)的np.array，A点和B点位置，注意是从A向B方向前进
        输出：
        is_empty: bool，True表示从A出发前进STEP_DISTANCE这段距离上没有障碍物
        newpoint: 从A点出发向B点方向前进STEP_DISTANCE距离后的新位置，如果is_empty为真，之后的代码需要把这个新位置添加到图中
        """
        len = np.linalg.norm(point_b - point_a)
        newpoint = (point_b - point_a) / len * STEP_DISTANCE + point_a
        is_empty = self.map.checkline(point_a.tolist(), newpoint.tolist())[0] == False
        if self.map.checkoccupy(newpoint):
            is_empty = False
        return is_empty, newpoint
