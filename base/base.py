#!/usr/bin/env python

ipython
Pip: python-pip 管理python包

打印： python3.0必须括号
print 'Hello %s\n' % __name__
print ('Hello %s\n' % __name__)
print '%s %s %s' %(mean, cov, n)

keyword(31):
and       del       from      not       while
as        elif      global    or        with
assert    else      if        pass      yield
break     except    import    print
class     exec      in        raise
continue  finally   is        return
def       for       lambda    try

scipy
matplotlib

https://docs.python.org/2/reference/index.html

import 
from import

b=array.array('H')
b=array('H')

array是build-in 模块，源码中只看到arraymodule.c，并没有array.py
模块中可能有方法，有类
类可以调用其构造函数，方法，常数等可以直接调用

tuple和list原生支持，array是库支持：
The Python Standard Librayr->Data Types

generator:
A function which returns an iterator
It looks like a normal function except that it contains yield statements for 
producing a series of values usable in a for-loop or that can be retrieved 
one at a time with the next() function

generator expression:
An expression that returns an iterator. It looks like a normal expression 
followed by a for expression defining a loop variable, range, and an optional 
if expression. The combined expression generates values for an enclosing function:

generator function:

def integers():
    int n = 0
    while True:
        yield n
        n += 1

The important thing in this example is the yield n statement. 
The function will return the value, and the next time it is called, it will continue from that point.

there is a lot of boilerplate
the logic has to be expressed in a somewhat convoluted way 


help(sum)  //查看sum的帮助
pip 工具

if __name__ == '__main__':

import zipfile  //zipfile 库模块

zipfile.ZipFile  //zipfile模块 ZipFile类

'''Read and write ZIP files.'''

Lib/zipfile.py


没参数：
def main(args = None):


类中方法，第一个参数必需为self（名字随意）

tempfile  标准库


同一张jpg，用ubuntu Image Viewer另存为bmp和winxp画图板另存为bmp，色调不一样
用matplotlib  array(Image.open('1.jpg')) 解的与winxp一致

cython 将pyx编译为.c，再将.c 编译为.so 文件
用于编写python的扩展模块，比如numpy就用到了

pylab

cv2.circle()  //怎样快速查看函数定义

print "%s" % points
只打印一部分：
[[  66.94115448  164.1769104 ]
 [  60.94831848  177.5411377 ]
 [  77.13156128  155.03884888]
 ..., 
 [ 225.3085022   359.10324097]
 [ 219.10496521  369.05072021]
 [ 219.92234802  381.0814209 ]]
怎么全部打印

Multivariate normal distribution  多变量高斯分布
mean and covariance (协方差)
numpy.random.multivariate_normal(mean, cov[, size])   //size 默认1

SVDecomp   SV Decomp SV分解

奇异值分解 (singular value decomposition,SVD)

调试
python -m pdb xx.py

python -m pdb xx.py 64.jpg  //与gdb不同，可以在这里直接加参数
用法与gdb类似

easy_install pip //Mac





from PIL import Image
from pylab import *

#im = array(Image.open('1.jpg').convert('L'))

#contour(im,origin='image')

figure()
figure()
#hist(im.flatten(),128)
show()


####
#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig, ax = plt.subplots()

x = np.arange(0, 2*np.pi, 0.01)        # x-array
line, = ax.plot(x, np.sin(x))

def animate(i):
    line.set_ydata(np.sin(x+i/10.0))  # update the data
    return line,

#Init only required for blitting to give a clean slate.
def init():
    line.set_ydata(np.ma.array(x, mask=True))
    return line,

ani = animation.FuncAnimation(fig, animate, np.arange(1, 200), init_func=init,
    interval=25, blit=True)
plt.show()

###

#!/usr/bin/env python

class test:
  def f():
    print "Hello"

if __name__ =='__init__':
  test.f()
  
  
###  hanoi
m=0

def hanoi(n, one, two, three):
  if n==1:
    move(one, three)
  else:
    hanoi(n-1, one, three, two)
    move(one, three)
    hanoi(n-1, two, one, three)

def move(x, y):
  global m
  m+=1
  print '%c --> %c  %d' %(x, y, m)

hanoi(5, 'A', 'B', 'C')


###  iterator
#!/usr/bin/env python

line_list = [' line1', '   line2', "     line3  i ", "   line4"]

# Generator expression -- returns iterator
stripped_iter = (line.strip() for line in line_list)

# List comprehension -- returns list
stripped_list = [line.strip() for line in line_list]

###function
#!/usr/bin/env python

def fun1(test):
  print 'Hello'
  def fun2(test1):
    print 'World'
  fun2(1)

fun1(1)


#!/usr/bin/python
import cv2
#import cv2.cv as cv
im = cv2.imread('./1.jpg')
h,w = im.shape[:2]
print h,w

cv2.imwrite('result.png',im)




#!/usr/bin/python

print "Hello World \n" *4
test = 5
print 'Gaojie %d \n' % (test*8)
print 'Gaojie %d \n' % test*4 # print 8 times

for i in range(1, 5):
	print i
else:
	print 'The loop is over'

print range(0, 100, 2)


