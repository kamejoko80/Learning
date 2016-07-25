import cv2
import numpy as np
import argparse

parser = argparse.ArgumentParser(description='Mirror: Flip the picture around the y axis.')
parser.add_argument('origin_folder', metavar='Orifol', type=str, nargs='+',
                    help='origin img folder.')
parser.add_argument('origin_txt', metavar='OriLog', type=str, nargs='+',
                    help='original head.txt path.')
parser.add_argument('mirror_folder', metavar='Mirfol', type=str, nargs='+',
                    help='mirror img folder.')
parser.add_argument('mirror_txt', metavar='MorLog', type=str, nargs='+',
				    default="./mirror.txt", help='mirror_head.txt path.')
args = parser.parse_args()


outf = open(args.mirror_txt[0], "w")
with open(args.origin_txt[0]) as inf:
    for l in inf:
        l = l.split(" ")
        img = cv2.imread(args.origin_folder[0] + l[0])
        img = cv2.flip(img, 1)
        #print l[0]
        l[0] = "mirror_" + l[0]
        cv2.imwrite(args.mirror_folder[0] + l[0], img)
        l[1] = str(- float(l[1]))
        s = " ".join(l)
        outf.write(s)

