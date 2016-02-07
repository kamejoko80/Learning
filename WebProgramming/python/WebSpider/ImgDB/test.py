import os
import imgDB

filename = '1.jpg'

img_db = imgDB.imgdb("images.db")
if not img_db.check_exist(filename):
    img_db.insert(filename)
#else:
#    os.remove(filename)

print img_db.count()

