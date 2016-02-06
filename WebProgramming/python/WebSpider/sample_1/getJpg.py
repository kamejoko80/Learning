#!/usr/bin/python
# Filename: getJpg.py
'''This is a sample for grep .JPG file from mm.taobao.com

This comes from http://cuiqingcai.com/1001.html
The picture site URL is http://mm.taobao.com/json/request_top_list.htm
'''

import urllib
import urllib2
import re
import tagProc
import os

class Spider:
    '''The Spider to get JPG

    spider = Spider()
    initialize the class with the website URL.
    spider.savePagesInfo(m, n)
    grep JPG method. get pictures from page m to n
    '''
    def __init__(self, url):
        self.siteURL = url
        self.tagProc = tagProc.tagProc()

    # get the Index page
    def getPage(self, pageIndex):
        url = self.siteURL + "?page=" + str(pageIndex)
        request = urllib2.Request(url)
        response = urllib2.urlopen(request)
        return response.read().decode('gbk')

    # get the page content
    def getContents(self, pageIndex):
        page = self.getPage(pageIndex)
        pattern = re.compile('<div class="list-item".*?pic-word.*?<a href="(.*?)".*?<img src="(.*?)".*?<a class="lady-name.*?>(.*?)</a>.*?<strong>(.*?)</strong>.*?<span>(.*?)</span>',re.S)
        items = re.findall(pattern, page)
        contents = []
        for item in items:
            contents.append([item[0], item[1], item[2], item[3], item[4]])
        return contents

    def getDetailPage(self, infoURL):
        response = urllib2.urlopen(infoURL)
        return response.read().decode('gbk')
    
    def getBrief(self, page):
        pattern = re.compile('<div class="mm-aixiu-content".*?>(.*?)<!--',re.S)
        result = re.search(pattern, page)
        return self.tagProc.replace(result.group(1))

    def getAllImg(self, page):
        pattern = re.compile('<div class="mm-aixiu-content".*?>(.*?)<!--',re.S)
        content = re.search(pattern, page)
        patternImg = re.compile('<img.*?src="(.*?)"',re.S)
        images = re.findall(patternImg,content.group(1))
        return images
        
    def saveImgs(self, images, name):
        number = 1
        print u"discover",name,u"sum up",len(images),u" pictures"
        for imageURL in images:
            splitPath = imageURL.split('.')
            fTail = splitPath.pop()
            if len(fTail) > 3:
                fTail = "jpg"
            fileName = name + "/" + str(number) + "." + fTail
            self.saveImg(imageURL,fileName)
            number += 1
    
    def saveIcon(self,iconURL,name):
        splitPath = iconURL.split('.')
        fTail = splitPath.pop()
        fileName = name + "/icon." + fTail
        self.saveImg(iconURL,fileName)
        
    
    def saveBrief(self,content,name):
        fileName = name + "/" + name + ".txt"
        f = open(fileName,"w+")
        print u"save her Brief in",fileName
        f.write(content.encode('utf-8'))
    
    def saveImg(self,imageURL,fileName):
        u = urllib.urlopen(imageURL)
        data = u.read()
        f = open(fileName, 'wb')
        f.write(data)
        print u"save a picture as",fileName
        f.close()
        
    def mkdir(self,path):
        path = path.strip()
        isExists=os.path.exists(path)
        if not isExists:
            print u"make a directory ",path
            os.makedirs(path)
            return True
        else:
            print path,'has made'
            return False
     
    
    def savePageInfo(self,pageIndex):
        contents = self.getContents(pageIndex)
        for item in contents:
        #item[0] Brief URL
        #item[1] icon URL
        #item[2] name
        #item[3] age
        #item[4] resident
            item[0] = 'http:' + item[0]
            item[1] = 'http:' + item[1]
        
            print u"discover a model named",item[2],u"age",item[3],u",her resident is",item[4]
            print u"save the infomation of",item[2]
            print u"her personal page is",item[0]
            detailURL = item[0]
            detailPage = self.getDetailPage(detailURL)
            brief = self.getBrief(detailPage)
            images = self.getAllImg(detailPage)
            self.mkdir(item[2])
            self.saveBrief(brief,item[2])
            self.saveIcon(item[1],item[2])
            self.saveImgs(images,item[2])
            
    def savePagesInfo(self,start,end):
        for i in range(start,end+1):
            print u"find",i,u"page"
            self.savePageInfo(i)


spider = Spider('http://mm.taobao.com/json/request_top_list.htm')
spider.savePagesInfo(2,10)

