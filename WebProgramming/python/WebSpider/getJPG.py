#!/usr/bin/python

#coding=utf-8

# Filename: getJPG.py
# Description: download .JPG pictures from website

import re
import urllib
import urllib2


# import coolielib
#
# filename = 'cookie.txt'
# cookie = cookielib.MozillaCookieJar(filename)
# handler = urllib2.HTTPCookieProcessor(cookie)
# opener = urllib2.build_opener(handler)
# postdata = urllib2.urlencode({'name':'name','password':'pwd'})
# result = opener.open(loginUrl, postdata)
# cookie.save(ignore_discard=True, ignore_expires=True)
## if open another time
## cookie.load('cookie.txt', ignore_discard=True, ignore_expires=True)
# result = opener.open(gradeUrl)

def setHttpProxy(url):
    proxy_handler = urllib2.ProxyHandler({"http" : url})
    opener = urllib2.build_opener(proxy_handler)
    urllib2.install_opener(opener)

def getHtml(url):
    '''
    Get html 
    '''
    
    ########################################
    # GET method
    #geturl = url + "?"+data
    #request = urllib2.Request(geturl)
    #response = urllib2.urlopen(request)
    #
    # POST method
    #values = {}
    #values['username'] = "youremail@gmail.com"
    #values['password'] = "password"
    #data = urllib.urlencode(values) 
    user_agent = 'Mozilla/5.0 (X11; Linux x86_64)'
    headers = {}
    headers['User-Agent'] = user_agent
    request = urllib2.Request(url, headers = headers)
    try:
        response = urllib2.urlopen(request, timeout = 10)
    except urllib2.URLError, e:
        if hasattr(e, "code"):
            print e.code
        if hasattr(e, "reason"):
            print e.reason
    else:
        print "OK"
    page = response.read()
    return page

def getImg(html):
    #reg = r'src="(http.*\.jpg)"'
    reg = r'http://pic.mmfile.net/\S*\.jpg'
    imgre = re.compile(reg)
    imglist = re.findall(imgre, html)
    x = 0
    for imgurl in imglist:
        urllib.urlretrieve(imgurl, '%s.jpg' % x)
        x += 1
    return imglist


if __name__ == '__main__':
    #proxy = 'http://127.0.0.1:8787'
    #setHttpProxy(proxy)
    url = 'http://www.mzitu.com/'
    #url = 'https://mm.taobao.com/'
    #url = 'https://www.google.com'
    #url = 'http://www.baidu.com/'
    html = getHtml(url)


    #print html
    print getImg(html)
