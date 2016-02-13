from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import pandas as pd

fig     = plt.figure()
ax      = fig.add_subplot(111, projection='3d')

table = pd.read_table('points_out.yml' , sep=' ')
x = table.values[range(0,156000,100)  ,0]
y = table.values[range(0,156000,100),1]
z = table.values[range(0,156000,100),2]
# x =[1,2,3,4,5,6,7,8,9,10]
# y =[5,6,2,3,13,4,1,2,4,8]
# z =[2,3,3,3,5,7,9,11,9,10]



ax.scatter(x, y, z, c='r', marker='o')

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()