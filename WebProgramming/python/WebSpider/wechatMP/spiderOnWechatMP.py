#! /usr/bin/python
# Filename: getImgFromWechatMP.py
# Description:

import os
import sys
import bs4
import urllib
import urllib2
import requests

def get_soup(url, tries = 1):
    '''
    Get soup object of the url
    '''
    user_agent = 'Mozilla/5.0 (X11; Linux x86_64)'
    headers = {}
    headers['User-Agent'] = user_agent
    request = requests.Session()
    #request.proxies = {"http": "http://127.0.0.1:8787",}

    try:
        #request = urllib2.Request(url, headers = headers)
        #response = urllib2.urlopen(request, timeout = 10)
        #page = response.read()
        page = request.get(url, headers = headers).content
        soup = bs4.BeautifulSoup(page)
        return soup
    except:
        print "Can not connect %s" % url
        if (tries - 1) > 0:
            print "try another time, remain try times %d" % (tries - 1)
            return get_soup(url, tries - 1)
        else:
            return -1

def get_images(soup):
    '''
    save the picture from the page "url" to "filename"
    '''
    global title
    global save_path
    counter = 1
    p_tags = soup.find_all('p')
    for p in p_tags:
        img_tags = p.find_all('img')
        for img in img_tags:
            img_url = img.get('data-src')
            if img_url != None:
                if img_url.endswith("png"):
                    filename = os.path.join(save_path, title + str(counter) + '.png')
                elif img_url.endswith("gif"):
                    filename = os.path.join(save_path, title + str(counter) + '.gif')
                elif img_url.endswith("jpeg") or img_url.endswith("jpg"):
                    filename = os.path.join(save_path, title + str(counter) + '.jpg')
                else:
                    filename = os.path.join(save_path, title + str(counter))

                if not os.path.exists(filename):
                    urllib.urlretrieve(img_url, filename)
                    print "......%d" % counter
                else:
                    print title + str(counter), "has saved, pass",
                counter += 1

def get_script(soup):
    global title
    global save_path
    script = os.path.join(save_path, title + '.txt')
    if not os.path.exists(script):
        with open(script, 'wb') as f:
            for p_tag in soup.find_all('p'):
                for span_tag in p_tag.find_all('span'):
                    if span_tag.string != None:
                        print span_tag.string,
                        f.write(span_tag.string.encode('utf8'))
                for br_tag in p_tag.find_all('br'):
                    print '\n'
                    f.write('\n\n')
                    

def get_title(soup):
    '''
    get the title of the artical
    '''
    title = soup.title.string.replace('/', ',')
    return title

def get_MPID(soup):
    MPID = soup.find_all('strong', class_='profile_nickname')[0].string
    MPID = MPID.replace('/', ',')
    return MPID

if __name__ == '__main__':
    # TODO: avoid no sys.argv[1]
    url = sys.argv[1]
    print "\nDownloading Img from:\n %s" % url
    
    soup = get_soup(url)
    
    MPID = get_MPID(soup)
    print "The Wechat MP ID is %s" % MPID
    if not os.path.exists(MPID):
        os.mkdir(MPID)
    title = get_title(soup)
    print "The page title is %s" % title
    save_path = os.path.join(MPID, title)
    if not os.path.exists(save_path):
        os.mkdir(save_path)
    
    get_images(soup)
    get_script(soup)





