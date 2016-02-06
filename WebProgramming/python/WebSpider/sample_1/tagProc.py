#!/usr/bin/python
# Filename: tagProc.py

import re

class tagProc:
    # remove Img tag
    removeImg = re.compile('<img.*?>| {1,7}|&nbsp;')
    # remove hyperlink tag
    removeAddr = re.compile('<a.*?>|</a>')
    # replace newline tag as \n
    replaceLine = re.compile('<tr>|<div>|</div>|</p>')
    # relace table tag as \t
    replaceTD = re.compile('<td>')
    # replace br tag as \n
    replaceBR = re.compile('<br><br>|<br>')
    # remove other tags
    removeExtraTag = re.compile('<.*?>')
    # remove blank lines 
    removeNoneLine = re.compile('\n+')
    def replace(self,x):
        x = re.sub(self.removeImg,"",x)
        x = re.sub(self.removeAddr,"",x)
        x = re.sub(self.replaceLine,"\n",x)
        x = re.sub(self.replaceTD,"\t",x)
        x = re.sub(self.replaceBR,"\n",x)
        x = re.sub(self.removeExtraTag,"",x)
        x = re.sub(self.removeNoneLine,"\n",x)
        return x.strip()


