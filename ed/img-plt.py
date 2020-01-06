import numpy as np
import matplotlib.pyplot as plt # plt 用于显示图片
import matplotlib.image as mpimg # mpimg 用于读取图片
from matplotlib.pyplot import MultipleLocator
from mpl_toolkits.mplot3d import Axes3D

np.set_printoptions(threshold=100000)

img = mpimg.imread('S.jpg') # (51, 61, 4)
print('Input image shape:', img.shape)
'''
plt.imshow(img)
plt.axis('off')
plt.show()
'''

# 转成灰度
gray=np.dot(img[...,:3], [0.299, 0.587, 0.114])
print('Gray shape:', gray.shape)
'''
plt.imshow(gray, cmap='Greys_r')
plt.axis('off')
plt.show()
'''

x_major_locator=MultipleLocator(1)
y_major_locator=MultipleLocator(1)

# 创建一个三维的绘图工程
ax = plt.subplot(111, projection='3d')
ax.set_zlabel('Z')  # 坐标轴
ax.set_ylabel('Y')
ax.set_xlabel('X')

plt.xlim(-0.5, 50)
plt.ylim(-0.5, 50)

'''
ax.xaxis.set_major_locator(x_major_locator)
ax.yaxis.set_major_locator(y_major_locator)
'''

#  将数据点分成三部分画，在颜色上有区分度
rows = gray.shape[0]
cols = gray.shape[1]
print('Rows: ', rows, 'Cols: ', cols)
for r in range(0, rows):
    colors = ('b', 'c', 'g', 'k', 'm', 'r', 'w', 'y')
    z = gray[r]
    # print('Row(', r, ')', z)
    for c in range(0, cols):
        # 绘制数据点
        '''
        x = [c for i in range(0, cols)] 
        y = [i for i in range(0, cols)]

        print('x: ', x)
        print('y: ', y)
        print('z: ', z[:cols])

        ax.scatter(x, y, z[:cols], c=colors[r%8])
        '''
        if z[c] > 200:
            ax.scatter(c, r, z[c], c='b')
        else:
            ax.scatter(c, r, z[c], c='r')

plt.show()

