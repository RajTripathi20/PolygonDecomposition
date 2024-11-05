import sys
import networkx as nx
import matplotlib
import matplotlib.pyplot as plt
for file_id in range(1,2):
########################
#file_id = "1"
#file_id = sys.argv[1]
########################


  G=nx.Graph()
  #f=open("visualisation_ans.txt","r")
  f=open("Visualisations/Inputs/visualisationDemo"+str(file_id)+"_ans.txt","r")
  n=int(f.readline())
  pointLabels=[]
  for i in range(n):
    label,x,y=f.readline().split()
    pointLabels.append(label)
    G.add_node(i+1,pos=(float(x),float(y)))

  for i in range(n):
    adjacencyMatricLineVector=f.readline().split()
    for j in range(n):
      if(j>=i):
        if(adjacencyMatricLineVector[j]=='1'):
          G.add_edge(i+1,j+1)

  pos=nx.get_node_attributes(G,'pos')
  nx.draw(G,pos, node_size=30,font_size=5)

  for key, value in pos.items():
    x,y=value
    #plt.text(x,y+0.3,s="("+str(x)+","+str(y)+")",horizontalalignment='center')
    #plt.text(x+0.01,y-0.1,s=pointLabels[key-1],horizontalalignment='center')

  # plt.savefig("visualisation_ans.png", dpi='figure', format="PNG")
  figure = plt.gcf() # get current figure
  figure.set_size_inches(20, 15)
  plt.savefig("Visualisations/visualisationDemo"+str(file_id)+"_ans.png", dpi='figure', format="PNG")
  plt.clf()
