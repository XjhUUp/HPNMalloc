import matplotlib.pyplot as plt
import pandas as pd
import sys
import numpy as np


#定义颜色变量
color = ['c', 'b', 'g', 'r', 'm', 'y', 'k', 'w']


def func1(file_path,pic_name):
    df = pd.read_table(file_path, header=None)
    nvm_writes =[]
    dram_writes=[]
    nvm_percentage=[]
    for i in range(len(df[0])):
        nvm_writes.append(int(df[0][i].split(' ')[0]))
        dram_writes.append(int(df[0][i].split(' ')[1]))
        nvm_percentage.append(float(df[0][i].split(' ')[3])*100)
    # 设置图形
    fig, ax1 = plt.subplots()

    # 绘制柱状图
    bar_width = 0.35
    index = np.arange(len(nvm_writes))
    bar1 = ax1.bar(index, nvm_writes, bar_width, label='NVM Writes', color='b')
    bar2 = ax1.bar(index + bar_width, dram_writes, bar_width, label='DRAM Writes', color='g')

    ax1.set_xlabel('Number of malloc(ten thousand)')
    ax1.set_ylabel('Write Counts')
    ax1.set_title('NVM and DRAM Writes')
    ax1.set_xticks(index + bar_width / 2)
    ax1.set_xticklabels([f'{(i + 1)}' for i in range(len(nvm_writes))])
    ax1.legend(loc='upper left')

    # 添加百分比数据的双坐标轴
    ax2 = ax1.twinx()
    ax2.set_ylabel('DRAM Write Percentage (%)')
    line = ax2.plot(index, nvm_percentage, 'o-', color='deepskyblue', label='DRAM Write Percentage')
    ax2.tick_params(axis='y')
    ax2.legend(loc='upper right')
    # 显示图形
    #plt.show()  
    pic_name='./exp_pic/hpn'+pic_name+'.png'
    plt.savefig(pic_name,bbox_inches='tight',dpi=300)






 

   



def func2(file_path1,file_path2,pic_name):
    # file_path = "hpn_res2.txt"
    # file_path1 = "wa_res2.txt"

    df1 = pd.read_table(file_path1, header=None)
    df2 = pd.read_table(file_path2, header=None)

    x1 = []
    y1 = []

    x2=[]
    y2=[]
    for i in range(len(df1[0])):
        x1.append(int(df1[0][i].split(' ')[0]))
        y1.append(float(df1[0][i].split(' ')[1]))

    for i in range(len(df2[0])):
        x2.append(int(df2[0][i].split(' ')[0]))
        y2.append(float(df2[0][i].split(' ')[1]))





    plt.scatter(x1,y1,c=color[1],label='HPN',alpha=0.9)
    plt.scatter(x2,y2,c=color[3],label='WA',alpha=0.2)

    #给图像加图例。 图例是集中于地图一角或一侧的地图上各种符号和颜色所代表内容与指标的说明，有助于更好的认识地图
    plt.legend()
    #plt.title("wear_test",fontdict={'size': 13,'color':  'k'})
    plt.xlabel('number of writes to the basic unit(64B)')
    plt.ylabel('number of blocks')
    plt.savefig(pic_name,bbox_inches='tight',dpi=300)
    #plt.show()

def func3(file_path1,file_path2):
   # 创建数据
    df1 = pd.read_table(file_path1, header=None)
    df2 = pd.read_table(file_path2, header=None)
    for i in range(len(df1[0])):
        categories = ['HPNMalloc', 'CPMA']
        # data_set1 = [10, 15, 20]  # 第一组数据
        # data_set2 = [8, 12, 18]   # 第二组数据
        # data_set3 = [15, 10, 25]  # 第三组数据
        # data_set4 = [12, 14, 20]  # 第四组数据
        data_set1 = []  # 第一组数据
        data_set2 = []   # 第二组数据
        data_set3 = []  # 第三组数据
        data_set4 = []  # 第四组数据

        data_set1.append(float(df1[0][i].split(' ')[0]))
        data_set1.append(float(df2[0][i].split(' ')[0]))
        #data_set1.append(float(df3[0][i].split(' ')[0]))

        data_set2.append(float(df1[0][i].split(' ')[1]))
        data_set2.append(float(df2[0][i].split(' ')[1]))
        #data_set2.append(float(df3[0][i].split(' ')[1]))

        data_set3.append(float(df1[0][i].split(' ')[2]))
        data_set3.append(float(df2[0][i].split(' ')[2]))
        #data_set3.append(float(df3[0][i].split(' ')[2]))

        data_set4.append(float(df1[0][i].split(' ')[3]))
        data_set4.append(float(df2[0][i].split(' ')[3]))
        #data_set4.append(float(df3[0][i].split(' ')[3]))

        

        # 设置柱状图宽度
        bar_width = 0.2

        #  设置柱状图的位置
        bar_positions1 = np.arange(len(categories))
        bar_positions2 = bar_positions1 + bar_width
        bar_positions3 = bar_positions2 + bar_width
        bar_positions4 = bar_positions3 + bar_width

        # 绘制柱状图
        plt.bar(bar_positions1, data_set1, width=bar_width, label='pmem0')
        plt.bar(bar_positions2, data_set2, width=bar_width, label='pmem1')
        plt.bar(bar_positions3, data_set3, width=bar_width, label='pmem2')
        plt.bar(bar_positions4, data_set4, width=bar_width, label='pmem3')

        # 设置轴标签和图例
        plt.ylabel('Write Counts')
        plt.xticks(bar_positions2, categories)  # 使用第二组位置的刻度作为标签位置
        plt.legend()

        # 显示图形
        #plt.show()
        pic_name='./exp_pic/wb_hpn'+str(i+20)+'.png'
        plt.savefig(pic_name,bbox_inches='tight',dpi=300)
        plt.cla()

def func4(file_path1,file_path2,pic_name):
    df1 = pd.read_table(file_path1, header=None)
    df2 = pd.read_table(file_path2, header=None)
    x=[1, 2, 3, 4, 5,6,7,8,9,10]
    y=[]
    for i in range(len(df1[0])):
        y.append(float(df1[0][i].split(' ')[8]))
    
    # 创建子图对象
    fig, ax = plt.subplots()
 
    # 绘制第一条曲线
    ax.plot(x, y, 'o-',label='HPN')
    y=[]
    for i in range(len(df2[0])):
        y.append(float(df2[0][i].split(' ')[8]))
    # 绘制第二条曲线
    ax.plot(x, y, 'o-',label='CPMA')
    # 设置标题、x轴和y轴的标签
    plt.xlabel('Number of malloc(ten thousand)')
    plt.ylabel('Variance')

    
 
    # 添加图例
    legend = ax.legend(loc='upper right',bbox_to_anchor=(1, 1))
    frame = legend.get_frame()
    frame.set_facecolor('white')

    pic_name='./exp_pic/hpn'+pic_name+'.png'
    plt.savefig(pic_name,bbox_inches='tight',dpi=300)
 
    # 显示图形
    # plt.show()

def func6(file_path1,file_path2,file_path3,pic_name):
    df1 = pd.read_table(file_path1, header=None)
    df2 = pd.read_table(file_path2, header=None)
    df3 = pd.read_table(file_path3, header=None)
   # df4 = pd.read_table(file_path4, header=None)
    x=[1, 2, 3, 4, 5,6,7,8,9,10]
    y=[]
    for i in range(len(df1[0])):
        y.append(float(df1.iloc[i, 0]))
    
    # 创建子图对象
    fig, ax = plt.subplots()
 
    # 绘制第一条曲线
    ax.plot(x, y, 'o-',label='HPN')
    y=[]
    for i in range(len(df2[0])):
        y.append(float(df2.iloc[i, 0]))
    # 绘制第二条曲线
    ax.plot(x, y, 'o-',label='WA')
    y=[]
    for i in range(len(df3[0])):
        y.append(float(df3.iloc[i, 0]))
    ax.plot(x, y, 'o-',label='NVM')
    # y=[]
    # for i in range(len(df4[0])):
    #     y.append(float(df4.iloc[i, 0]))
    # ax.plot(x, y, 'o-',label='CPMA')
    plt.xticks(x)


    # 设置标题、x轴和y轴的标签
    plt.xlabel('Number of malloc(ten thousand)')
    plt.ylabel('Average time(us)')

    
 
    # 添加图例
    legend = ax.legend(loc='upper right',bbox_to_anchor=(0.2, 1))
    frame = legend.get_frame()
    frame.set_facecolor('white')

    pic_name='./exp_pic/hpn'+pic_name+'.png'
    plt.savefig(pic_name,bbox_inches='tight',dpi=300)
 
    # 显示图形
    # plt.show()

def func7(file_path1,file_path2,file_path3,pic_name):
    df1 = pd.read_table(file_path1, header=None)
    df2 = pd.read_table(file_path2, header=None)
    df3 = pd.read_table(file_path3, header=None)
   # df4 = pd.read_table(file_path4, header=None)
    # x=[10, 20, 30, 40, 50,60,70,80,90,100,110,120,130,140,150]
    x=[1, 2, 3, 4, 5,6,7,8,9,10]
    y=[]
    for i in range(len(df1[0])):
        y.append(float(df1.iloc[i, 0]))
    
    # 创建子图对象
    fig, ax = plt.subplots()
 
    # 绘制第一条曲线
    ax.plot(x, y, 'o-',label='HPN')
    y=[]
    for i in range(len(df2[0])):
        y.append(float(df2.iloc[i, 0]))
    # 绘制第二条曲线
    ax.plot(x, y, 'o-',label='WA')
    y=[]
    for i in range(len(df3[0])):
        y.append(float(df3.iloc[i, 0]))
    ax.plot(x, y, 'o-',label='NVM')
    # y=[]
    # for i in range(len(df4[0])):
    #     y.append(float(df4.iloc[i, 0]))
    # ax.plot(x, y, 'o-',label='CPMA')
    plt.xticks(x)


    # 设置标题、x轴和y轴的标签
    plt.xlabel('Number of malloc(ten thousand)')
    plt.ylabel('Average time(us)')

    
 
    # 添加图例
    legend = ax.legend(loc='upper right',bbox_to_anchor=(0.2, 1))
    frame = legend.get_frame()
    frame.set_facecolor('white')

    pic_name='./exp_pic/hpn'+pic_name+'.png'
    plt.savefig(pic_name,bbox_inches='tight',dpi=300)
 
    # 显示图形
    # plt.show()

if __name__=="__main__":
    eval_num = int(sys.argv[1])
    if eval_num == 1:
        file_path=str(sys.argv[2])
        pic_name=str(sys.argv[3])
        func1(file_path,pic_name)
    if eval_num == 2:
        file_path1=str(sys.argv[2])
        file_path2=str(sys.argv[3])
        pic_name=str(sys.argv[4])
        func2(file_path1,file_path2,pic_name)
    if eval_num == 3:
        file_path1=str(sys.argv[2])
        file_path2=str(sys.argv[3])
        func3(file_path1,file_path2)
    if eval_num == 4:
        file_path1=str(sys.argv[2])
        file_path2=str(sys.argv[3])
        pic_name=str(sys.argv[4])
        func4(file_path1,file_path2,pic_name)
    if eval_num == 6:
        file_path1=str(sys.argv[2])
        file_path2=str(sys.argv[3])
        file_path3=str(sys.argv[4])
        # file_path4=str(sys.argv[5])
        pic_name=str(sys.argv[5])
        func6(file_path1,file_path2,file_path3,pic_name)
    if eval_num == 7:
        file_path1=str(sys.argv[2])
        file_path2=str(sys.argv[3])
        file_path3=str(sys.argv[4])
        # file_path4=str(sys.argv[5])
        pic_name=str(sys.argv[5])
        func7(file_path1,file_path2,file_path3,pic_name)





