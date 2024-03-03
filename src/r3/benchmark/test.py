import sys
import pandas as pd

 
 
if __name__=="__main__":
    file_path1=str(sys.argv[1])
    file_path2=str(sys.argv[2])
    file_path3=str(sys.argv[3])
    file_path4=str(sys.argv[4])
    df1 = pd.read_table(file_path1, header=None)
    df2 = pd.read_table(file_path2, header=None)
    df3 = pd.read_table(file_path3, header=None)
    df4 = pd.read_table(file_path4, header=None)
    y1=[]
    for i in range(len(df1[0])):
        y1.append(float(df1.iloc[i, 0]))
    y2=[]
    for i in range(len(df2[0])):
        y2.append(float(df2.iloc[i, 0]))
    y3=[]
    for i in range(len(df3[0])):
        y3.append(float(df3.iloc[i, 0]))
    y4=[]
    for i in range(len(df4[0])):
        y4.append(float(df4.iloc[i, 0]))
    ratio=0.0
    length=len(df1[0])
    for i in range(len(df1[0])):
        ratio+=((y1[i]-y2[i])/y2[i])
        #print(ratio)
    ratio=(ratio/length)
    print(ratio)
    ratio=0.0
    for i in range(len(df1[0])):
        ratio+=((y1[i]-y3[i])/y3[i])
    ratio=(ratio/length)
    print(ratio)
    ratio=0.0
    for i in range(len(df1[0])):
        ratio+=((y1[i]-y4[i])/y4[i])
    ratio=(ratio/length)
    print(ratio)

        
    
