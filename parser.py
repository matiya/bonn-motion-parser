import sys
import getopt
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation


#data = sys.stdin.readlines()
#print "Counted", len(data), "lines."

fig = plt.figure()
ax = plt.axes(xlim=(0, 300), ylim=(0, 300))
line, = ax.plot([], [], lw=2)


def main(argv):
  try:
      opts, args = getopt.getopt(argv,"oan")
  except getopt.GetoptError:
      print "Usage:"
      print 'parser.py [-OPTION] <file>'
      sys.exit(2)
  for opt, arg in opts:
      if opt == '-o':
         print 'plotting just the scatter plot of one node'
         oneNode(argv)
         sys.exit()
      elif opt == '-a':
        print("plotting all the nodes")
        allNodes(argv)
        sys.exit()
      elif opt in ("-n", "--animate"):
         animated()
  #if str(argv) == "['scenario1.visplot1']":
   # onenode(str(argv))


def oneNode(fileName):
#plot one node
  ax = fig.add_subplot(1,1,1)
  a = np.loadtxt(str(fileName[1]))
  x = a[:,0]
  y = a[:,1]
  ax.plot(x,y,'bo--')
  plt.show()

def allNodes(fileName):
#plots all the nodes

  f = open(str(fileName[1]))
  i = 0
  nodes = []
  for line in f:
    #print(type(float(line.split()[1])))
    #print(float(line.split()[1]))
    #print line
    nodes.append(line)
    i += 1
  #print(nodes[0])
  print("totalNodes: "+str(len(nodes[0].split())))
  
  nodeContainer = [] #contains all the nodes
  for index in range(i):
    #initialize all the nodes to zero
    node = np.zeros((len(nodes[index].split())/3,3))
    nodeContainer.append(node)

    
  indexOfNodes = 0
  listOfCoords = []

  for element in nodes:
    indexOfCoords = 0
    #list of the coordenates of node "element" as [t0,x0,y0,t1...]
    listOfCoords = element.split()
    i = 0 #line of an specific node, so: [tn,xn,yn]
    while indexOfCoords != len(listOfCoords):
      #assigns the t (time) coordinate of node element
      #if(indexOfNodes < 2):
        #print("t: "+str(listOfCoords[indexOfCoords]))
        #print("x: "+str(listOfCoords[indexOfCoords+1]))
        #print("y: "+str(listOfCoords[indexOfCoords+2]))
        #print("-------------------------------")
      nodeContainer[indexOfNodes][i][0] = listOfCoords[indexOfCoords]
      nodeContainer[indexOfNodes][i][1] = listOfCoords[indexOfCoords + 1]
      nodeContainer[indexOfNodes][i][2] = listOfCoords[indexOfCoords + 2]
      indexOfCoords += 3
      i += 1
    indexOfNodes += 1
    
  print("first row: ")
  print(nodeContainer[0][:,0])
  print("second row: ")
  print(nodeContainer[0][:,1])
  ax = fig.add_subplot(1,1,1)
  x0 = nodeContainer[0][:,1]
  y0 = nodeContainer[0][:,2]
  x1 = nodeContainer[1][:,1]
  y1 = nodeContainer[1][:,2]
  ax.plot(x0,y0,'bo--', x1, y1, 'go--')
  plt.show()


# initialization function: plot the background of each frame
def init():
    line.set_data([], [])
    return line,

# animation function.  This is called sequentially
def animate(i):
    #x = np.linspace(0, 2, 1000)
    #y = np.sin(2 * np.pi * (x - 0.01 * i))
    a = np.loadtxt("scenario1.visplot1")
    #a = np.loadtxt(str(fileName[1]))
    #x = a[:,0]
    #y = a[:,1]
    print i
    x = a[i][0]
    y = a[i][1]
    line.set_data(x, y)
    return line,

# call the animator.  blit=True means only re-draw the parts that have changed.
def animated():
  anim = animation.FuncAnimation(fig, animate, init_func=init,                             frames=10, interval=2, blit=True)
  plt.show()


if __name__ == "__main__":
   main(sys.argv[1:])
