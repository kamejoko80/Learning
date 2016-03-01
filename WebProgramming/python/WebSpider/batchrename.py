#!/usr/bin/python
# filename: batchRename.py

import os

# local spider
path = os.path.join('xnxx.com')

# for dir_name in os.listdir(path):
#     for file_name in os.listdir(os.path.join(path, dir_name)):
#         img = os.path.join(path, dir_name, file_name)
#         os.rename(img, os.path.join(path, dir_name + '_' + file_name))
#     os.rmdir(os.path.join(path, dir_name))


for file_name in os.listdir(path):
    dir_name = os.path.join(path, file_name[:4])
    if not os.path.exists(dit_name):
        os.mkdir(dir_name)
    os.rename(file_name, os.path.join(dir_name, file_name))

