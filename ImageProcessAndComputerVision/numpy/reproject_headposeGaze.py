# -*- coding: utf-8 -*-
"""
Created on Fri Oct 10 13:13:37 2014

@author: sugano
"""

import os
import glob

import cv2
import numpy as np

import csv

import scipy.io as sio
import matplotlib.pyplot as plt
from common import *

def estimateHeadPose(landmarks, face_model, camera, distortion, iterate=True):
    ret, rvec, tvec = cv2.solvePnP(face_model, landmarks, camera, distortion, flags=cv2.CV_EPNP)
    ## further optimize
    if iterate:
        ret, rvec, tvec = cv2.solvePnP(face_model, landmarks, camera, distortion, rvec, tvec, True)
    return rvec, tvec

def drawLandmarks(img, landmarks, color):
    for p in landmarks:
        cv2.line(img, (int(p[0,0])-5, int(p[0,1])), (int(p[0,0])+5, int(p[0,1])), color, 2)
        cv2.line(img, (int(p[0,0]), int(p[0,1])-5), (int(p[0,0]), int(p[0,1])+5), color, 2)

def drawPose(img, r, t, cam, dist):
    modelAxes = np.array([
        np.array([0., -20., 0.]).reshape(1,3),
        np.array([50., -20., 0.]).reshape(1,3),
        np.array([0., -70., 0.]).reshape(1,3),
        np.array([0., -20., -50.]).reshape(1,3)
    ])
    projAxes, jac = cv2.projectPoints(modelAxes, r, t, cam, dist)

    cv2.line(img, (int(projAxes[0,0,0]), int(projAxes[0,0,1])), (int(projAxes[1,0,0]), int(projAxes[1,0,1])), (0,255,255), 2)
    cv2.line(img, (int(projAxes[0,0,0]), int(projAxes[0,0,1])), (int(projAxes[2,0,0]), int(projAxes[2,0,1])), (255,0,255), 2)
    cv2.line(img, (int(projAxes[0,0,0]), int(projAxes[0,0,1])), (int(projAxes[3,0,0]), int(projAxes[3,0,1])), (255,255,0), 2)

def drawGaze(img, face, hr, ht, gc, cam, dist, color):
    ## compute estimated 3D positions of the landmarks
    hR = cv2.Rodrigues(hr)[0]
    face_cam = np.dot(hR, face) + ht
    re = 0.5*(face_cam[:,0] + face_cam[:,1]).reshape((3,1))
    le = 0.5*(face_cam[:,2] + face_cam[:,3]).reshape((3,1))

    ## compute gaze vectors
    rv = gc - re
    lv = gc - le
    rv /= np.linalg.norm(rv)
    lv /= np.linalg.norm(lv)

    ## gaze positions to be displayed
    gv_length = 100
    rg = gv_length*rv + re
    lg = gv_length*lv + le

    ## project them to the image space
    camPts = np.array([re, rg, le, lg])
    imgPts, jac = cv2.projectPoints(camPts, np.zeros(3), np.zeros(3), cam, dist)

    cv2.line(img, (int(imgPts[0,0,0]), int(imgPts[0,0,1])),  (int(imgPts[1,0,0]), int(imgPts[1,0,1])), color, 2)
    cv2.line(img, (int(imgPts[2,0,0]), int(imgPts[2,0,1])), (int(imgPts[3,0,0]), int(imgPts[3,0,1])), color, 2)

if __name__ == '__main__':
    metadata_root = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/Original'
    data_root = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/Original'
    landmarks_root = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data'

    sbj_list = os.listdir(data_root)
    sbj_list.sort()

    show_result = True #False

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

            csv_path = os.path.join(pts_dir, 'pose_all.csv')
            #if os.path.exists(csv_path):
            #    continue

            csv_file = open(csv_path, 'wb')
            csv_writer = csv.writer(csv_file)

            annotation_file = open(os.path.join(pts_dir, 'annotation.txt'), 'r')

            for idx, img_name in enumerate(img_list):
                base, ext = os.path.splitext(img_name)
                if ext != '.jpg' and base[0] == '.':
                    continue
                annotation = annotation_file.readline().split(' ')
                annotation = np.asarray(annotation)
                annotation = [float(x) for x in annotation]
                #landmarks = [float(x) for x in landmarks]
                #landmarks = np.asarray(landmarks)
                #landmarks = np.reshape(landmarks, (6, 1, 2))
                #plt.plot(landmarks[:,0], landmarks[:,1], 'ro')
                #plt.show()
                #pts_path = os.path.join(pts_dir, base+'_det_0.pts')
                #if not os.path.exists(pts_path):
                #    continue

                ## head pose estimation
                #landmarks = readPtsFile(pts_path)
                #hr, ht = estimateHeadPose(landmarks, facePts, cam, dist, False)

                hr = np.reshape(annotation[29:32], (3, 1))
                ht = np.reshape(annotation[32:], (3, 1))
                #raw_input("enter to continue")
                target_3D = np.reshape(annotation[26:29],(3, 1))

                #raw_input("stop")
                ## visualize
                proj, jac = cv2.projectPoints(facePts, hr, ht, cam, dist)
                #err = np.linalg.norm((landmarks - proj).flatten())
                img = cv2.imread(os.path.join(data_root, sbj_dir, date_dir, img_name))
                #drawLandmarks(img, landmarks, (255,0,0))
                drawLandmarks(img, proj, (0,0,255))
                drawPose(img, hr, ht, cam, dist)
                drawGaze(img, face, hr, ht, target_3D, cam, dist, (0,255,0))

                img = cv2.resize(img, (960, 540))
                vis_path = os.path.join(pts_dir, base+'_vis.jpg')
                cv2.imwrite(vis_path, img)

                if show_result:
                    cv2.imshow("debug", img)
                    key = cv2.waitKey()
                    if key == 27:
                        break

                ## write result
                data = [str(val) for val in np.concatenate([hr, ht, target_3D]).flatten()]
                #csv_writer.writerow([base, str(err)] + data)

            csv_file.close()

    if show_result:
        cv2.destroyAllWindows()
