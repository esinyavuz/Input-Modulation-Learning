'''
Code for plotting the glomerular activity.
by Esin Yavuz
University of Sussex
email: e.yavuz@sussex.ac.uk
'''

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import rcParams
import sys

colors= ['r', 'g', 'b', 'y','k','c','m','orange','violet','teal','brown','grey','olive','palegreen','pink','purple']

#Listof all glomeruli that we want.
glomeruli=[15,17,18,19,21,23,
24,25,27,28,29,30,
33,35,36,37,38,39,
42,43,47,48,49,52,
60,62,108,121,135,142]#need to be ordered

glomeruli_toplot=[15,17,23]#need to be ordered

fig_width = 12.0  
fig_height = 5.25 
fig_size = [fig_width, fig_height] #in inches

params = {'backend': 'SVG', #change to SVG for vector. or GDK for gimp finishing
           'axes.labelsize': 12,
           'axes.titlesize': 12,
           'axes.edgecolor': 'k',
           'font.size': 12,
           'text.usetex': 'true',
           'figure.figsize': fig_size,
           'savefig.dpi': 600,
           'font.family': 'arial',
           'axes.linewidth': 1.2, 
           'axes.facecolor': 'white',
           'lines.linewidth': 2.0,
           'lines.markersize': 2.5,
           'lines.markeredgewidth': 0.5,
           'legend.numpoints': 1,
           'legend.columnspacing':1.0,
           'legend.labelspacing' : 0.15,
           'grid.linestyle':'.',
           'grid.linewidth': 0.5,
           'grid.color':'lightgrey',
           'patch.linewidth': 0,
           'svg.fonttype': 'path'
}

rcParams.update(params)
nAL=150
inglom = 5 #also 1 interneuron from LN

sdffile=open('sdffile')#this is the file created by tools/st2asdf_mult

sdfarray = []
	
for line in sdffile.readlines():
	a=line.split(" ")
	pt = []
	for k in range(len(a)-1):
		pt.append(a[k]) # #neurons
	sdfarray.append(pt)

sdfnp=np.array(sdfarray,dtype=float)
sdfnp[:,0]=sdfnp[:,0]/1000
sdfnp[:,1:]=sdfnp[:,1:]*1000

print(np.shape(sdfnp))

numrow = 5
numcol = 6 
ymax=140
ymin=-10
xmin=0
xmax=sdfnp[-1,0]
print('xmax: ',xmax)
plt.figure(1)

########################
protofile=open('protofile')#This is the modelname.proto file
stimdur=[]
stimid=[]
for line in protofile:
	a=line.split(" ")
	#following could break down if stim on/off is not in order
	if (a[1] == 'odor'):
		if (a[5] == '1\n'):
			tmparr=[]
			tmparr.append(a[0])
		if (a[5] == '-1\n'):#&&(len(a)==0)
			tmparr.append(a[0])
			stimdur.append(tmparr)
			stimid.append(int(a[3]))

stimdurnp=np.array(stimdur,dtype=float)
stimdurnp=stimdurnp/1000
####################
toplot_cntr=0
parr = []
for p in np.arange(1,12):
	ax=plt.subplot(4,3,p)
	ax.spines['right'].set_visible(False)
	ax.spines['top'].set_visible(False)
	ax.xaxis.set_ticks_position('bottom')
	ax.yaxis.set_ticks_position('left')
	for kind in range(np.shape(stimdurnp)[0]):
		ax.axvspan(stimdurnp[kind,0],stimdurnp[kind,1],color=colors[stimid[kind]])
		ax.set_ylim(ymin, ymax)
		ax.set_xlim(xmin, xmax)
	if (p %3 !=1):
		ax.set_yticklabels([])

for neuron in range(nAL):
	if (toplot_cntr<3):
		glomid = int (neuron / 5)
		print(toplot_cntr,glomid)
		if (glomeruli_toplot[toplot_cntr]==glomeruli[glomid]):
			if (neuron % 5 != 0):
				ax=plt.subplot(4,3,toplot_cntr+1)
				ax.plot(sdfnp[:,0],sdfnp[:,neuron+1],color='k')
				ax.set_ylim(ymin, ymax)
				ax.set_xlim(xmin, xmax)
			else:
				ax=plt.subplot(4,3,toplot_cntr+1)
				ax.plot(sdfnp[:,0],sdfnp[:,neuron+1],color='g',zorder=10)
				ax.set_ylim(ymin, ymax)
				ax.set_xlim(xmin, xmax)
				ax.set_title(glomeruli[glomid])

			if (neuron % 5 == 4):			
				toplot_cntr=toplot_cntr+1	
				parr.append(glomid)

			if (glomid %3 !=0):
				ax.set_yticklabels([])

			if (int(glomid/3)!=4):
				ax.set_xticklabels([])
		
ax=plt.subplot(4,3,10)
ax.plot(sdfnp[:,0],sdfnp[:,sdfnp.shape[1]-2],color='k')
ax.set_xlim(xmin, xmax)
ax.set_ylim(-1.5, 12)
ax=plt.subplot(4,3,11)
ax.plot(sdfnp[:,0],sdfnp[:,sdfnp.shape[1]-1],color='k')
ax.set_xlim(xmin, xmax)
ax.set_ylim(-1.5, 12)

ax.set_xlim(xmin, xmax)
print('xmax 1: ',xmax)
##############################
cmpfile=open('cmpfile')#this is the outputfile (outname/outname.out.cmp)

cmparray = []
	
for line in cmpfile.readlines():
	a=line.split(" ")
	pt = []
	for k in range(len(a)-1):
		pt.append(a[k]) # #neurons
	cmparray.append(pt)


cmpnp=np.array(cmparray,dtype=float)
cmpnp[:,0]=cmpnp[:,0]/1000

xmin=0
xmax=cmpnp[-1,0]

print('xmax 2: ',xmax)

for p in range(6):
	ax=plt.subplot(4,3,4+p)
	ax.set_xticklabels([])
	ax.set_xlim(xmin, xmax)
	ax.set_ylim(-1e-11, 5e-11)

for p in [7,8,9]:
	ax=plt.subplot(4,3,p)
	ax.set_ylim(15, 35)


ax=plt.subplot(4,3,4)
ax.set_ylabel('p ')
ax.plot(cmpnp[:,0],cmpnp[:,1],color='k') # glom 0

ax=plt.subplot(4,3,5)
ax.plot(cmpnp[:,0],cmpnp[:,4],color='k') # glom 1

ax=plt.subplot(4,3,6)
ax.plot(cmpnp[:,0],cmpnp[:,5],color='k') # glom 5


ax=plt.subplot(4,3,7)
ax.set_ylabel('g (pS)')
ax.plot(cmpnp[:,0],cmpnp[:,2]/1e-6,color='k')
ax=plt.subplot(4,3,8)
ax.plot(cmpnp[:,0],cmpnp[:,7]/1e-6,color='k')
ax=plt.subplot(4,3,9)
ax.plot(cmpnp[:,0],cmpnp[:,8]/1e-6,color='k')

for p in np.arange(4,10):
	ax=plt.subplot(4,3,p)
	ax2 = ax.twinx()
	ax2.plot(cmpnp[:,0],cmpnp[:,3],color='#445566',linewidth=1.5)
	ax2.yaxis.tick_right()

	ax2.spines['right'].set_visible(False)
	ax2.spines['top'].set_visible(False)
	ax2.xaxis.set_ticks_position('bottom')
	if (p %3 !=0):
		ax2.set_yticklabels([])

title= sys.argv[1]
print(title)

ax=plt.subplot(4,3,11)
ax.set_xlabel('time (s)')
ax=plt.subplot(4,3,1)
ax.set_ylabel("spike density (Hz)")
ax=plt.subplot(4,3,10)
ax.set_ylabel("spike density (Hz)")

plt.text(2,61.25,"glomerulus:")
plt.text(223.5,23.9,'reward',rotation=270)
plt.text(223.5,40.1,'reward',rotation=270)

plt.text(1,58,"A+",fontsize=11)
plt.text(9,58,"A+",fontsize=11)
plt.text(17,58,"A+",fontsize=11)
plt.text(25,58,"A+",fontsize=11)
plt.text(33,58,"A+",fontsize=11)
plt.text(41.1,58,"B-",fontsize=11)
plt.text(49,58,"A+",fontsize=11)
plt.text(57,58,"A+",fontsize=11)
plt.text(65,58,"A+",fontsize=11)

plt.text(-20,60.5,'a',fontsize=14)
plt.text(-20,42.1,'b',fontsize=14)
plt.text(-20,26.7,'c',fontsize=14)
plt.text(-20,10,'d',fontsize=14)

plt.text(2,10,'learner',fontsize=12)
plt.text(78,10,'non-learner',fontsize=12)

plt.subplots_adjust(left=0.09,right=0.95,bottom=0.1,top=0.93,wspace=0.08,hspace=0.2)

plt.show()
