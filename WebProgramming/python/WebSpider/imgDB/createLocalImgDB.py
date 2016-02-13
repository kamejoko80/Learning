#!/usr/bin/python
# filename: createLocalImgDB.py

import os
import imgDB
import time

def db_insert(filename):
    global img_db
    if not img_db.check_exist(filename):
        img_db.insert(filename)
    #else:
    #    os.remove(filename)
    print img_db.count()

# local spider
path = os.path.join('MeiZiTu')
img_db = imgDB.imgdb("images.db")
counter = 1

for dir_name in os.listdir(path):
    for file_name in os.listdir(os.path.join(path, dir_name)):
        img = os.path.join(path, dir_name, file_name)
        counter += 1
        if counter > 1220:
            db_insert(img)
            time.sleep(1)
