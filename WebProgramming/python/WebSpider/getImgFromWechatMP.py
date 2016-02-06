#! /usr/bin/python
# Filename: getImgFromWechatMP.py
# Description:

__author__ = HopingTang
__version__ = 0.1

import os
import sys
import bs4
import urllib
import urllib2

def get_soup(url, tries = 1):
    '''
    Get soup object of the url
    '''
    user_agent = 'Mozilla/5.0 (X11; Linux x86_64)'
    headers = {}
    headers['User-Agent'] = user_agent
    try:
        request = urllib2.Request(url, headers = headers)
        response = urllib2.urlopen(request, timeout = 10)
        page = response.read()
        soup = bs4.BeautifulSoup(page)
        return soup
    except:
        print "Can not connect %s" % url
        if (tries - 1) > 0:
            print "try another time, remain try times %d" % (tries - 1)
            return get_soup(url, tries - 1)
        else:
            return -1

def get_images(soup, title):
    '''
    save the picture from the page "url" to "filename"
    '''
    counter = 1
    p_tags = soup.find_all('p')
    for p in p_tags:
        img_tags = p.find_all('img')
        for img in img_tags:
            img_url = img.get('data-src')
            if img_url != None:
                filename = os.path.join(title, title + str(counter) + '.jpg')
                if not os.path.exists(filename):
                    urllib.urlretrieve(img_url, filename)
                    print "......%d" % counter, 
                else:
                    print title + str(counter), "has saved, pass"
                counter += 1

def get_title(soup):
    '''
    get the title of the artical
    '''
    title = soup.title.string
    return title

if __name__ == '__main__':
    
    help_str = '''\
This program can get img from Wechat MP.\n
./getImgFromWechatMP [option] [URL]\n
Options include:
--version : Prints the version number
--help    : Display this help
--title   : Costomise the title'''
    
    if len(sys.argv) == 1:
        print "No URL specified"
        url = raw_input("Input the URL")

    if len(sys.argv) == 2:
        if sys.argv[1].startswith('--'):
            option = sys.argv[1][2:]
                # fetch sys.argv[1] but without
                # the first two characters
            if option == 'version':
                print 'Version %s' % __version 
            elif option == 'help':
                print help_str
            else:
                print 'Unknown option.'
            sys.exit()
        else:
            url = sys.argv[1]

    if len(sys.argv) == 4:
        if sys.argv[1].startswith('--'):
            if sys.argv[1][2:] == 'title':
                title = sys.argv[2]
                url = sys.argv[3]
            else:
                print 'Unknown option'
                sys.exit()
        else:
            print help_str
            sys.exit()
    
    soup = get_soup(url)

    if title != None
        title = get_title(soup)
    print "\nDownloading Img from:\n %s" % url
    print "The page title is %s" % title
    if not os.path.exists(title):
        os.mkdir(title)
    
    get_images(soup, title)





