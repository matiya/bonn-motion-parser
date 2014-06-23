"""this was hacked together fast and dirty and is therefore
unmaintainable.
should only be used for debugging or not at all if possible
"""

#from watchdog.observers import Observer
#from watchdog.events import LoggingEventHandler
#from watchdog.events import FileSystemEventHandler
import sys
import os
import time
import getopt
from matplotlib.lines import Line2D
from pylab import *
from IPython.display import Image
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation


def allNodes(fileName):
  """plots all the nodes, the input is the decompressed file produced by bm"""
  print(fileName)
  f = open(str(fileName[0]),'r')
  obsf = open(str(fileName[1]),'r')
  
  i = 0
  j = 0
  k = 0
  nodes = []
  dataObs = []
  dataObs1 = []
  for line in f:
    nodes.append(line)
    i += 1
  for line in obsf:
    j=0
    for element in line.strip().split(','):
      if(j%2==0):
        print(type(element))
        try:
          dataObs.append( float(element) )
        except:
          pass
      else:
        try:
          dataObs1.append( float(element) )
        except:
          pass
      j+=1
  #plot obstacles
  for k in range(len(dataObs)):
    if k%2==0:
      plt.plot(dataObs[k:k+2],dataObs1[k:k+2], lw = 3)
      
  
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

# initialization function: plot the background of each frame
def init():
    for line in lineList:
        line.set_data([], [])
    #line.set_data([], [])
    #line2.set_data([], [])
    return lineList

# animation function.  This is called sequentially
def animate(i):
    data = []
    c = 0
    for l in range(nro*2):
        data.append(np.empty(i))
    for a in range(nro):
        for j in range(i):
            data[c][j] = points[a][j][1]
            data[c+1][j] = points[a][j][2]
        c += 2

    c = 0
    for a in range(nro):
      lineList[a].set_data(data[c],data[c+1])
      c+=2
    #line.set_data(data[0], data[1])
    return lineList 

if __name__ == "__main__":
  fig = plt.figure()
  ax = plt.axes(xlim=(0, 1000), ylim=(0, 1000))
  ax.grid()
  lineList = []
  points, nro = allNodes(["/home/default/workspace/repos/python/bonn-motion-parser/data/sc0",
                          "/home/default/workspace/repos/python/bonn-motion-parser/temp/obs"])
  for i in range(nro):
    linea, = ax.plot([], [], lw=2)
    lineList.append(linea)
  anim = animation.FuncAnimation(fig, animate, init_func=init,
                                 frames=900, interval=20, blit=True,
                                 repeat = True)
  plt.show()

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])


