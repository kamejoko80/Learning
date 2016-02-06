#!/usr/bin/python

#coding=utf-8

# Filename: getJPG.py
# Description: download .JPG pictures from website
# it comes from http://blog.csdn.net/wucl202000/article/details/46292539

import os
# import re
import bs4
import urllib
import urllib2


def get_soup(url):
    '''
    Get soup object
    '''
    
    user_agent = 'Mozilla/5.0 (X11; Linux x86_64)'
    headers = {}
    headers['User-Agent'] = user_agent
    request = urllib2.Request(url, headers = headers)
    response = urllib2.urlopen(request, timeout = 10)
    page = response.read()
    soup = bs4.BeautifulSoup(page)
    return soup

def get_pages(url):
    """
    get the number of pages
    """
    soup=get_soup(url)
    nums=soup.find_all('a',class_='page-numbers')
    pages=int(nums[-2].text)
    return pages
   
    
def get_menu(url):
    """
    obtain the name and address and add them to list
    """
    soup=get_soup(url)
    menu=[]
    menu_list=soup.find_all('a',target='_blank')
    for i in menu_list:
        result=i.find_all('img',class_='lazy')
        if result:
            name=result[0]['alt']
            address=i['href']
            menu.append([name,address])
    return menu

def get_links(url):
    """
    find out how many pages of the same topic
    """
    soup=get_soup(url)
    all_=soup.find_all('a')
    nums=[]
    for i in all_:
        span=i.find_all('span')
        if span:
            nums.append(span[0].text)
    print nums
    return nums[-2]

def get_image(url,filename):
    """
    save the picture from a page to 'filename'
    """
    soup=get_soup(url)
    image=soup.find_all('p')[0].find_all('img')[0]['src']
    urllib.urlretrieve(image,filename)
    
def main(page):
    """
    save page "page" in the site
    """
    print u'downloading page %d' % page
    page_url=url+'/page/'+str(page)
    menu=get_menu(page_url)
    print u'\nthere are %d topics in page %d\n' %(len(menu), page)
    for i in menu:
        dir_name=os.path.join('MeiZiTu',i[0])
        if not os.path.exists(dir_name):
            os.mkdir(dir_name)
        pic_nums=int(get_links(i[1]))
        print u'\n\n\n page %s has %d pictures\n' %(i[0],pic_nums)
        for pic in range(1,pic_nums+1):
            basename=str(pic)+'.jpg'
            filename=os.path.join(dir_name,basename)
            pic_url=i[1]+'/'+str(pic)
            if not os.path.exists(filename):
                print u'......%s' % basename,
                try:
                    get_image(pic_url,filename)
                except:
                    print "download picture %s fail" % basename
            else:
                print filename+u'has saved, so pass'

if __name__ == '__main__':
    url='http://www.mzitu.com/'
    pages=get_pages(url)
    print u'*************** In that site, there are %d pages ******************' %pages
    if not os.path.exists('MeiZiTu'):
        os.mkdir('MeiZiTu')
    page_start=input(u'Input the first page number:\n')
    page_end=input(u'Input the last page number:\n')
    if page_end > page_start:
        for page in range(page_start,page_end):
            main(page)
    elif page_end==page_start:
        main(page_end)
    else:
        print u"input error, start page must less then end page \n"


