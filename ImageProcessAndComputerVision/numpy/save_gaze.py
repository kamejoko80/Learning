# -*- coding: utf-8 -*-

import os
import glob
import cv2
import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt
from common import *

if __name__ == '__main__':
    metadata_root = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/Original'
    data_root = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/Original'
    landmarks_root = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data'
    crop_root = landmarks_root + '/Crop'

    sbj_list = os.listdir(data_root)
    sbj_list.sort()

    for sbj_dir in sbj_list:
        calib_dir = os.path.join(metadata_root, sbj_dir, 'Calibration')
        ## load face model
        face_path = os.path.join(landmarks_root, '6 points-based face model.mat')
        if not os.path.exists(face_path):
            continue
        face = sio.loadmat(face_path)['model']
        num_pts = face.shape[1]
        facePts = face.T.reshape(num_pts,1,3)

        print sbj_dir

        ## load calibration data
        cameraCalib = sio.loadmat(os.path.join(calib_dir, 'Camera.mat'))
        cam = cameraCalib['cameraMatrix']
        dist = cameraCalib['distCoeffs']

        ## scan image files...
        date_list = os.listdir(os.path.join(data_root, sbj_dir))
        date_list.sort()

        for date_dir in date_list:
            if date_dir == 'Calibration' or not os.path.isdir(os.path.join(data_root, sbj_dir, date_dir)):
                continue
            print date_dir
            img_list = os.listdir(os.path.join(data_root, sbj_dir, date_dir))
            img_list.sort()

            pts_dir = os.path.join(data_root, sbj_dir, date_dir)
            crop_dir = os.path.join(crop_root, sbj_dir, date_dir)

            log_path = os.path.join(crop_dir, 'eyePitchYaw.txt')
            log_file = open(log_path, 'w')

            annotation_file = open(os.path.join(pts_dir, 'annotation.txt'), 'r')

            for idx, img_name in enumerate(img_list):
                base, ext = os.path.splitext(img_name)
                if ext != '.jpg' or base[0] == '.':
                    continue
                annotation = annotation_file.readline().split(' ')
                annotation = np.asarray(annotation)
                try:
                    annotation = [float(x) for x in annotation]
                    hr = np.reshape(annotation[29:32], (3, 1))
                    ht = np.reshape(annotation[32:], (3, 1))
                    target_3D = np.reshape(annotation[26:29],(3, 1))
                    ## visualize
                    hR = cv2.Rodrigues(hr)[0]
                    re = 0.5*(face[:,0] + face[:,1]).reshape((3,1))
                    le = 0.5*(face[:,2] + face[:,3]).reshape((3,1))
                    target_head = np.dot(np.transpose(hR), target_3D - ht)

                    ## compute gaze vectors
                    rv = target_head - re
                    lv = target_head - le
                    ryaw = np.arctan(rv[0] / rv[2]) * 180 / 3.1415926
                    rpitch = np.arcsin(rv[1] / np.linalg.norm(rv)) * 180 / 3.1415926
                    lyaw = np.arctan(lv[0] / lv[2]) * 180 / 3.1415926
                    lpitch = np.arcsin(lv[1] / np.linalg.norm(lv)) * 180 / 3.1415926

                    ## write result
                    log_file.write(img_name+' '+str(rpitch[0])+' '+str(ryaw[0])+' '+str(lpitch[0])+' '+str(lyaw[0])+'\n')
                except ValueError:
                    print annotation[32:]

            log_file.close()
