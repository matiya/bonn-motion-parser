"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""
import sys
import os
import getopt
from matplotlib.lines import Line2D
from pylab import *
from IPython.display import Image
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

def allNodes(fileName):
  """plots all the nodes, the input is the decompressed file produced by bm"""
  f = open(str(fileName[1]))
  i = 0
  nodes = [] 
  for line in f:
    nodes.append(line)
    i += 1
  print("totalNodes: "+str(len(nodes)))
  n = len(nodes)
  nodeContainer = [] #contains all the nodes
  for index in range(i):
    #initialize all the nodes to zero
    node = np.zeros((len(nodes[index].split())/4,4))
    nodeContainer.append(node)

    
  indexOfNodes = 0
  listOfCoords = []

  for element in nodes:
    indexOfCoords = 0
    #list of the coordenates of node "element" as [t0,x0,y0,t1...]
    listOfCoords = element.split()
    i = 0 #line of an specific node, so: [tn,xn,yn]
    while indexOfCoords != len(listOfCoords):
      nodeContainer[indexOfNodes][i][0] = listOfCoords[indexOfCoords]
      nodeContainer[indexOfNodes][i][1] = listOfCoords[indexOfCoords + 1]
      nodeContainer[indexOfNodes][i][2] = listOfCoords[indexOfCoords + 2]
      nodeContainer[indexOfNodes][i][3] = listOfCoords[indexOfCoords + 3]
      indexOfCoords += 4
      i += 1
    indexOfNodes += 1
    
  return nodeContainer, n

# First set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax = plt.axes()
ax.grid()
points, nro = allNodes([1,"/home/default/workspace/repos/python/bonn-motion-parser/mobilityGenerator/build/scenariob0"])
lineList = []
for i in range(nro):
    linea, = ax.plot([], [], lw=2)
    lineList.append(linea)
line, = ax.plot([], [], lw=2)
line2, = ax.plot([], [], lw=2)
# initialization function: plot the background of each frame


def init():
    for line in lineList:
        line.set_data([], [])
    line.set_data([], [])
    line2.set_data([], [])
    return lineList

# animation function.  This is called sequentially


def animate(i):
    #print(nodes[i])
    x = np.linspace(0, 2, 1000)
    y = np.sin(2 * np.pi * (x - 0.001 * i))
    data = []
    c = 0
    for l in range(nro*2):
        data.append(np.empty(i))
    for a in range(nro):
        for j in range(i):
            #print (str(a+1)+", "+str(j) + " = " +str(points[a][j][2]))
            #if(a%2 == 0):
            #print(data)
            #rint(c)
            data[c][j] = points[a][j][1]
            data[c+1][j] = points[a][j][2]
            #print("\n")
        c += 2
 #           else:
#                data[a][j] = points[a][j][2]
    c = 0
    for a in range(nro):
      lineList[a].set_data(data[c],data[c+1])
      c+=2
      
    
        
    line.set_data(data[0], data[1])
    #print(points[1][i], points[1][i][2])
    #line2.set_data(y, x)
    #return data[0], data[1], data
    return lineList 

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=500, interval=20, blit=True)

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])

plt.show()
