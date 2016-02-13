#!/usr/bin/python
# Filename: imgDB.py

import os
import hashlib
import sqlite3

class imgdb:
    def __init__(self, db_name):
        self.db_name = 'images.db'

    def get_size(self, filename):
        return os.path.getsize(filename)

    def get_md5(self, filename):
        return hashlib.md5(open(filename,'rb').read()).hexdigest()

    def connect_init(self):
        # connect to the image database
        # if it doesn't exists, the database will be created
        self.connection = sqlite3.connect(self.db_name)
        self.cursor = self.connection.cursor()

    def connect_close(self):
        # close the cursor
        self.cursor.close()
        # commit and close the connection
        self.connection.commit()
        self.connection.close()

    def create_table(self):
        '''create the 'images' table of the database.

        you should run the method in the first time.'''
        self.connect_init()
        command = 'create table images (size integer, md5 varchar(33))'
        self.cursor.execute(command)
        self.connect_close()
    
    def insert(self, filename):
        '''insert into the database.
        
        get the size and md5 of the file,
        and insert them into the images database.'''
        self.connect_init()
        size = self.get_size(filename)
        md5 = self.get_md5(filename)
        command = 'insert into images (size, md5) values (%d, \'%s\')'\
                  % (size, md5)
        self.cursor.execute(command)
        # print self.cursor.rowcount
        self.connect_close()

    def check_exist(self, filename):
        '''check if the image has exited in the database.
        '''
        self.connect_init()
        size = self.get_size(filename)
        md5 = self.get_md5(filename)
        command = 'select * from images where size=? and md5=?'
        self.cursor.execute(command, (size, md5))
        check = len(self.cursor.fetchall())
        
        self.connect_close()
        return bool(check)

    def count(self):
        '''count the number of images in the database.
        '''
        self.connect_init()
        command = 'select count(*) from images'
        self.cursor.execute(command)
        count = self.cursor.fetchall()[0][0]
        
        self.connect_close()
        return count

# RUN THIS IN THE FIRST TIME:
def create_db(db_name):
    img_db = imgdb(db_name)
    img_db.create_table() 

if __name__ == "__main__": 
    # RUN THIS IN THE FIRST TIME:
    img_db = imgdb('images.db')
    img_db.create_table() 

