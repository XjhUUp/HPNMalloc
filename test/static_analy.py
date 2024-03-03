import matplotlib.pyplot as plt
import pandas as pd

#读入文件
file_path = "hpn.txt"
df = pd.read_table(file_path, header=None)

#定义 x  y变量
x = []
y = []

#定义颜色变量
color = ['c', 'b', 'g', 'r', 'm', 'y', 'k', 'w']

#用for循环将文件中的值赋值给x，y
for i in range(len(df[0])):
    x.append(int(df[0][i].split(' ')[0]))
    y.append(int(df[0][i].split(' ')[1]))

#画图
plt.scatter(x, y, c=color[1], edgecolors='r')
plt.show()
