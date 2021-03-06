import cv2
import os
#from matplotlib.colors import LogNorm
import matplotlib.pyplot as plt
import numpy as np
import argparse
from shutil import copyfile
parser = argparse.ArgumentParser()
parser.add_argument("path")
args = parser.parse_args()
print 'Path', args.path
folder = args.path
folder = "/".join(folder.split('/')[:-1])
folder += '/'
print folder
items_file = open(args.path)
bins = []
data = []
pitch_list = []
yaw_list = []
roll_list = []

step = 5 #degree

for l in items_file:
    sp = l.split(' ')
    img_name = sp[0]
    img_meta = img_name.split('_')
    pId = img_meta[0][3:]
    caseId = img_meta[1][2:]
    camId = int(img_meta[2][3:-4])
    pitch = float(sp[1])
    yaw = float(sp[2])
    roll = float(sp[3])
    pitch_list.append(pitch)
    yaw_list.append(yaw)
    roll_list.append(roll)
    data.append((pitch, yaw, roll, img_name))

#plt.hist2d(yaw_list, pitch_list, bins=(yaw_binNum, pitch_binNum))
#plt.colorbar()
#plt.show()
pitch_binNum = int((max(pitch_list) - min(pitch_list)) / step)
yaw_binNum = int((max(yaw_list) - min(yaw_list)) / step)
roll_binNum = int((max(roll_list) - min(roll_list)) / step)

H, [xedges, yedges ,zedges] = np.histogramdd((pitch_list, yaw_list, roll_list), bins=(pitch_binNum,yaw_binNum,roll_binNum))

for i in range(0, pitch_binNum):
    bins.append([])
    for j in range(0, yaw_binNum):
        bins[i].append([])
        for k in range(0, roll_binNum):
            bins[i][j].append([])
for d in data:
    pitch = d[0]
    yaw = d[1]
    roll = d[2]
    binx = int((pitch - xedges[0]) / step)
    biny = int((yaw - yedges[0]) / step)
    binz = int((roll - zedges[0]) / step)
    binx = binx - 1 if binx == pitch_binNum else binx	
    biny = biny - 1 if biny == yaw_binNum else biny	
    binz = binz - 1 if binz == roll_binNum else binz	
    try:
        bins[int(binx)][int(biny)][int(binz)].append(d[3])
    except IndexError:
        print "ERROR: bin index out of range."
        print int(binx), int(biny), int(binz)
print bins

for i in range(0, pitch_binNum):
    for j in range(0, yaw_binNum):
        for k in range(0, roll_binNum):
            pstr = str(int(i * step + xedges[0]))[:5]
            ystr = str(int(j * step + yedges[0]))[:5]
            rstr = str(int(k * step + zedges[0]))[:5]
            directory = folder+'/headpose_bins/p'+pstr+'y'+ystr+'r'+rstr
            #directory = folder+'/lefteye_bins/p'+pstr+'y'+ystr+'r'+rstr
            continue
            if len(bins[i][j][k]):
                print pstr, ystr,rstr, len(bins[i][j][k])
                if not os.path.exists(directory):
                    os.makedirs(directory)
                for f in bins[i][j][k]:
                    copyfile(folder + '/head/' + f, directory + '/' + f)
                    #copyfile(folder + '/eyes/left_' + f, directory + '/' + f)

