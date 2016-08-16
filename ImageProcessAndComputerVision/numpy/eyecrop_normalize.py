# -*- coding: utf-8 -*-
"""
Created on Fri Oct 17 15:47:11 2014
@author: sugano
"""
import os
import cv2
import numpy as np
import csv
import scipy.io as sio

def normalizeData(img, face, hr, ht, cam, dist):
    ## (target) camera parameters
    focal_new = 960
    distance_new = 600
    roiSize = (60, 36)
    ## undistort image
    img_u = cv2.undistort(img, cam, dist)
    img_u = cv2.cvtColor(img_u, cv2.COLOR_BGR2GRAY)
    cv2.imshow("eye_ori", img_u)
    cv2.waitKey(0)

    ## compute estimated 3D positions of the landmarks
    ht = ht.reshape((3,1))
    #gc = gc.reshape((3,1))
    hR = cv2.Rodrigues(hr)[0]
    m_temp = np.dot(hR, face);
    Fc = np.dot(hR, face) + ht
    re = 0.5*(Fc[:,0] + Fc[:,1]).reshape((3,1)) # the eye center
    le = 0.5*(Fc[:,2] + Fc[:,3]).reshape((3,1))
    ## normalize each eye
    data = []
    for et in zip([re, le]):
        ## ---------- normalize image ----------
        distance = np.linalg.norm(et)
        z_scale = distance_new/distance
        cam_new = np.array([
            [focal_new, 0, roiSize[0]/2],
            [0, focal_new, roiSize[1]/2],
            [0, 0, 1.0],
        ])
        scaleMat = np.array([
            [1.0, 0.0, 0.0],
            [0.0, 1.0, 0.0],
            [0.0, 0.0, z_scale],
        ])

        hRx = hR[:,0]
        forward = (et/distance).reshape(3)
        down = np.cross(forward, hRx) # just X axis
        down /= np.linalg.norm(down)
        right = np.cross(down, forward)
        right /= np.linalg.norm(right)


        rotMat = np.c_[right, down, forward].T

        warpMat = np.dot(np.dot(cam_new, scaleMat), np.dot(rotMat, np.linalg.inv(cam)))
        temp_1 = np.dot(cam_new, scaleMat)
        temp_2 = np.dot(rotMat, np.linalg.inv(cam))
        temp_3 = np.linalg.inv(cam)

        img_warped = cv2.warpPerspective(img_u, warpMat, roiSize)
        img_warped = cv2.equalizeHist(img_warped)

        ## ---------- normalize rotation ----------
        cnvMat = rotMat
        hRnew = np.dot(cnvMat, hR)
        hrnew = cv2.Rodrigues(hRnew)[0]
        htnew = np.dot(cnvMat, et)
        print hrnew
        print htnew
        cv2.imshow("eye"+str(np.int8(z_scale*100)), img_warped)
        cv2.waitKey(0)
        ## ---------- normalize gaze vector ----------
        gcnew = np.dot(cnvMat, gc)
        gvnew = gcnew - htnew
        gvnew = gvnew/np.linalg.norm(gvnew)
        data.append([img_warped, hrnew, gvnew, pupil_norm])
        data = []

    return data

if __name__ == '__main__':
        face_path = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/6 points-based face model.mat'
        face = sio.loadmat(face_path)['model']

        #print sbj_dir
        ## load calibration data
        cameraCalib = sio.loadmat('/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/Original/p01/Calibration/Camera.mat')
        cam = cameraCalib['cameraMatrix']
        dist = cameraCalib['distCoeffs']
        #date_list = os.listdir(os.path.join(data_root, sbj_dir))
        #date_list.sort()

        for num_i in [1]:
            #pose_reader = csv.reader(open(os.path.join(data_root, sbj_dir, 'pose_generic.csv'), 'rb'))
            pose_reader = np.array([-0.008508, 0.074914, 0.065328, -12.188643, -16.965384, 496.795410]).reshape((6,1))

            ## face alignment certainty
            #certainty_reader = csv.reader(open(os.path.join(pts_dir, 'certainty.csv'), 'rb'))
            #certainty_indexed = {}
            #for row in certainty_reader:
            #    certainty_indexed[row[0]] = float(row[1])

            ## data storage
            imgs_r = []
            poses_r = []
            gazes_r = []
            pupil_r = []
            imgs_l = []
            poses_l = []
            gazes_l = []
            pupil_l = []

            ## metadata
            certainties = []
            errors = []
            filenames = []

            for indx, row in enumerate(pose_reader):

                filepath = '/home/sensetime/Documents/Work/DATASET/MPIIGaze/Data/Original/p01/day43/1408606382.jpg'
                #filepath = '/home/sensetime/Downloads/002.jpg'
                img = cv2.imread(filepath)
                hr = np.array([float(val) for val in pose_reader[0:3]])
                ht = np.array([float(val) for val in pose_reader[3:6]])
                #gc = np.array([float(val) for val in row[8:11]])

                #print filepath
                print cam
                print dist
                #raw_input("s")
                data = normalizeData(img, face, hr, ht, cam, dist)

                imgs_r.append(data[0][0])
                poses_r.append(data[0][1].reshape(3))
                gazes_r.append(data[0][2].reshape(3))
                pupil_r.append(data[0][3].reshape(2))

                imgs_l.append(data[1][0])
                poses_l.append(data[1][1].reshape(3))
                gazes_l.append(data[1][2].reshape(3))
                pupil_l.append(data[1][3].reshape(2))

                #certainties.append(certainty_indexed[filename])
                errors.append(float(row[1]))
                filenames.append([int(val) for val in filename.split('_')])

