# /usr/bin/python
# encoding=utf-8


import requests
from bs4 import BeautifulSoup


def download_page(url):
    request = requests.Session()
    request.proxies = {"http": "http://127.0.0.1:8787",}
    headers = {
        'User-Agent' : 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36',
    }
    data = request.get(url, headers = headers).content
    return data


if __name__ == "__main__":
    
