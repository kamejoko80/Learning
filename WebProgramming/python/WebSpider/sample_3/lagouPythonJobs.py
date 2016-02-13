#! /usr/bin/python
# encoding=utf-8

# tutorial url : http://zhuanlan.zhihu.com/xlz-d/20430122

import json
import request
from bs4 import BeautifulSoup

Request_URL = 'http://www.lagou.com/jobs/positionAjax.json?'


def parse():
    headers = {
        'User-Agent' : 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36',
        'Referer' : 'http://www.lagou.com/zhaopin/Python/?labelWords=label',
        'X-Requested-With' : 'XMLHttpRequest',
        }
    request_data = {
        'first' : 'true',
        'pn' : '1',
        'kd' : 'Python',
        }
    json_obj = json.loads("{data}".format(data = request_data))
    json_str = json.dumps(json_obj)
