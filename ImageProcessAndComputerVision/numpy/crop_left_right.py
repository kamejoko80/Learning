# -*- coding: utf-8 -*-

import os
import cv2
import glob
import numpy as np
import scipy.io as sio
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from common import *

def obtain_bbox(landmark):
    left = min(landmark[:,0,0])
    right = max(landmark[:,0,0])
    width = right - left
    x_mid = (right + left) / 2
    top = min(landmark[:,0,1])
    bottom = max(landmark[:,0,1])
    height = bottom - top
    y_mid = (top + bottom) / 2
    return [int(x) for x in [x_mid-width, x_mid+width, y_mid-2*height, y_mid+2*height]]

def rotate(center, x, y, angle):
    x = x - center[0]
    y = y - center[1]
    return [x * np.cos(angle) - y * np.sin(angle) + center[0], x * np.sin(angle) + y + np.cos(angle) + center[1]]

def obtain_rot_bbox(center, angle, landmark):
    left = rotate(center, landmark[0,0,0], landmark[0,0,1], angle)
    right = rotate(center, landmark[3,0,0], landmark[3,0,1], angle)
    width = right[0] - left[0]
    x_mid = (left[0] + right[0]) / 2
    y_mid = (left[1] + right[1]) / 2
    return [int(x) for x in [x_mid - width, x_mid + width, y_mid - width / 2, y_mid + width / 2]]

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
        monitorPose = sio.loadmat(os.path.join(calib_dir, 'monitorPose.mat'))
        mon_r = monitorPose['rvects']
        mon_t = monitorPose['tvecs']
        mon_R = cv2.Rodrigues(mon_r)[0]
        screenSize = sio.loadmat(os.path.join(calib_dir, 'screenSize.mat'))
        mon_wmm = screenSize['width_mm'][0,0]
        mon_wpx = screenSize['width_pixel'][0,0]
        mon_hmm = screenSize['height_mm'][0,0]
        mon_hpx = screenSize['height_pixel'][0,0]
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

            log_path = os.path.join(pts_dir, 'eyePitchYaw.txt')
            #if os.path.exists(csv_path):
            #    continue
            #log_file = open(log_path, 'w')

            annotation_file = open(os.path.join(pts_dir, 'annotation.txt'), 'r')

            for idx, img_name in enumerate(img_list):
                base, ext = os.path.splitext(img_name)
                if ext != '.jpg' or base[0] == '.':
                    continue
                try:
                    annotation = annotation_file.readline().split(' ')
                    annotation = np.asarray(annotation)
                    annotation = [float(x) for x in annotation]

                    leye_screen = np.reshape(annotation[0:12],(6,1,2))
                    reye_screen = np.reshape(annotation[12:24],(6,1,2))

                    #leye_bbox =  obtain_bbox(leye_screen)
                    #reye_bbox = obtain_bbox(reye_screen)

                    img = cv2.imread(os.path.join(data_root, sbj_dir, date_dir, img_name))
                    rotateAngle = np.arctan((reye_screen[3,0,1] - leye_screen[0,0,1]) / (reye_screen[3,0,0] - leye_screen[0,0,0]))
                    rot_mat = cv2.getRotationMatrix2D((leye_screen[0,0,0], leye_screen[0,0,1]), rotateAngle * 180 / 3.1415926, 1.0)
                    img = cv2.warpAffine(img, rot_mat, (img.shape[1], img.shape[0]), flags=cv2.INTER_LINEAR)

                    leye_bbox = obtain_rot_bbox((leye_screen[0,0,0], leye_screen[0,0,1]), rotateAngle, leye_screen)
                    reye_bbox = obtain_rot_bbox((leye_screen[0,0,0], leye_screen[0,0,1]), rotateAngle, reye_screen)

                    leye = img[leye_bbox[2] : leye_bbox[3], leye_bbox[0] : leye_bbox[1], :]
                    reye = img[reye_bbox[2] : reye_bbox[3], reye_bbox[0] : reye_bbox[1], :]
                    #raw_input('s')

                    crop_dir = os.path.join(crop_root, sbj_dir, date_dir)
                    if not os.path.exists(crop_dir):
                        os.makedirs(crop_dir)
                    cv2.imwrite(os.path.join(crop_dir, 'lefteye'+img_name), leye)
                    cv2.imwrite(os.path.join(crop_dir, 'righteye'+img_name), reye)


                except ValueError:
                        print annotation
            #log_file.close()

