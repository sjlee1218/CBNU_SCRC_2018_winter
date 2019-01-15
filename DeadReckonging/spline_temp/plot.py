import numpy as np
import pylab as plt
import pandas as pd

origin_list=[]
with open("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/just_lat_lon_tab.txt", 'r', encoding="utf8") as original:
  while True:
    line = original.readline()
    if not line:
      break
    lon = float(line.split("\t")[0])
    #print(lon)
    lat = float(line.split("\t")[1])
    temp=[lon,lat]
    origin_list.append(temp)

df=pd.DataFrame(origin_list)
x=np.array(df[0])
y=np.array(df[1])

plt.figure(figsize=(20, 20))
plt.title("original", fontsize=25)
plt.scatter(x,y)
plt.show()

Bez_list = []
with open("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/by_bezier.txt", 'r', encoding="utf8") as Bezier:
  while True:   
    line = Bezier.readline()
    if not line:
      break
    lon = float(line.split("\t")[0])
    #print(lon)
    lat = float(line.split("\t")[1])
    temp=[lon,lat]
    Bez_list.append(temp)

df=pd.DataFrame(Bez_list)
x=np.array(df[0])
y=np.array(df[1])

plt.figure(figsize=(20, 20))
plt.title("Bezier",fontsize=25)
plt.scatter(x,y)
plt.show()

Bsp_list = []
with open("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/by_Bspline.txt", 'r', encoding="utf8") as Bs:
  while True:   
    line = Bs.readline()
    if not line:
      break
    lon = float(line.split("\t")[0])
    #print(lon)
    lat = float(line.split("\t")[1])
    temp=[lon,lat]
    Bsp_list.append(temp)
    
df=pd.DataFrame(Bsp_list)
x=np.array(df[0])
y=np.array(df[1])

plt.figure(figsize=(20, 20))
plt.title("BSpline",fontsize=25)
plt.scatter(x,y)
plt.show()

catmull_list = []
with open("/Users/leeseungjoon/Desktop/GitHub_SJLEE/CBNU_SCRC_2018_winter/DeadReckonging/spline_temp/by_catmull.txt", 'r', encoding="utf8") as cm:
  while True:   
    line = cm.readline()
    if not line:
      break
    lon = float(line.split("\t")[0])
    #print(lon)
    lat = float(line.split("\t")[1])
    temp=[lon,lat]
    catmull_list.append(temp)

df=pd.DataFrame(catmull_list)
x=np.array(df[0])
y=np.array(df[1])

plt.figure(figsize=(20, 20))
plt.title("Catmull",fontsize=25)
plt.scatter(x,y)
plt.show()
