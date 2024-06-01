import matplotlib.pyplot as plt

# 数据
data = """
502 136.296
503 135.769
504 127.865
505 126.305
506 124.922
507 126.553
508 120.821
509 101.256
510 95.7639
511 95.3084
512 7.54453e-08
513 4.5408e-08
514 5.19099e-08
"""

# 解析数据
x = []
y = []
for line in data.strip().split('\n'):
    xi, yi = map(float, line.split())
    x.append(xi)
    y.append(yi)

# 绘制图形
plt.figure(figsize=(10, 6))
plt.plot(x, y, marker='o', linestyle='-', color='b')
plt.xlabel('r')
plt.ylabel('||x_r - x||_1')
plt.title('Result')
plt.grid(True)
plt.savefig('./output.svg', format='svg')
plt.show()
