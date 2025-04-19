# PDC 2025 第一次作业 CUDA Reduction

本项目对比CUDA中不同并行归约实现的性能，包括一个基础实现和优化版本。

## 项目结构

- `main.cu`: 基准测试主程序(包含核函数调用、运行时间测量和计算结果比对)
- `reduction.cuh`: 头文件(包含常量、工具函数和核函数声明)
- `reduction.cu`: 核函数实现
- `Makefile`: 编译文件

## Baseline 设计

项目包含一个简单的归约核函数(`reductionNaive`)，其特点：
- 每个线程处理`NAIVE_BLOCK_DIM`(8)个元素
- 使用原子操作进行全局内存上的结果累加

## 学生需实现的任务

在`reduction.cu`中实现优化版本的Reduction核函数：

1. 优化方向：
   - 使用共享内存并实现内存合并访问
   - 避免共享内存访问冲突
   - 使用Warp指令

## 编译与运行

1. 编译代码并运行：
```bash
make all
```

2. 单独编译代码或单独运行：
```bash
make build
make run
```

3. 清理：
```bash
make clean
```

## 预期结果

成功的实现应：
1. 通过CPU参考结果验证
2. 相比基础版本减少执行时间
3. 保持相同的输出格式
