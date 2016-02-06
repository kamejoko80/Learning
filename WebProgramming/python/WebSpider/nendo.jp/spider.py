#!/usr/bin/python
# Filename: spider.py
# Description: download pictures from website

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
    response = urllib2.urlopen(request)#, timeout = 10)
    page = response.read()
    soup = bs4.BeautifulSoup(page)
    return soup

def get_pages(url):
    """
    get the number of pages
    """
    soup = get_soup(url)
    pages = []
    for li_tag in soup.find_all('li', class_='cat-item'):
        name = li_tag.find_all('a')[0].string
        address = li_tag.find_all('a')[0]['href']
        pages.append([name, address])
    return pages
   
    
def get_menu(url):
    """
    obtain the name and address and add them to list
    """
    soup = get_soup(url)
    menu = []
    for menu_list in soup.find_all('a',rel = 'bookmark'):
        name = menu_list.string
        address = menu_list['href']
        menu.append([name, address])
    return menu

def get_links(url):
    """
    find out how many pages of the same topic
    """
    soup = get_soup(url)
    pic_url = []
    for img_tag in soup.find_all('img'):
        pic_url.append(img_tag['src'])
    return pic_url

def get_image(url,filename):
    """
    save the picture from a page to 'filename'
    """
    urllib.urlretrieve(url, filename)

def main(page, dir_page):
    menu = get_menu(page[1])
    print '\n\nthere are %d topics in page %s' %(len(menu), page[1])
    
    for i in menu:
        dir_name = os.path.join(dir_page, i[0])
        if not os.path.exists(dir_name):
            os.mkdir(dir_name)
        if len( get_menu(i[1])) != 0:
            main(i, dir_name)
        pic_url = get_links( i[1]) 
        pic_nums = len(pic_url)
        print u'\n\n\n page %s has %d pictures\n' %(i[0], pic_nums)
        
        counter = 1
        for url in pic_url:
            basename = str(counter) + '.jpg'
            filename = os.path.join(dir_name, basename)
            counter += 1
            if not os.path.exists(filename):
                print '......%s' % basename,
                try:
                    get_image(url, filename)
                except:
                    print "download picture %s fail" % basename
            else:
                print '......' + basename + ' has saved, so pass',

if __name__ == '__main__':
    url = 'http://www.nendo.jp/en/works/'
    pages = get_pages(url)
    print ' In that site, there are %d pages, they are %s' % (len(pages), pages)
    for page in pages:
        dir_page = os.path.join(page[0])
        if not os.path.exists(dir_page):
            os.mkdir(dir_page)
        main(page, dir_page)
