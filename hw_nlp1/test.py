
import os
import requests
from bs4 import BeautifulSoup
import re



url = "https://www.xqb5200.com/"
header = {"User-Agent":
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
req = requests.get(url = url, headers = header)
req.encoding = "gbk"
html = req.text
bes = BeautifulSoup(html,"lxml")
# texts = bes.find("div", id="list")
print(bes)
for tag in bes.find_all(class_="s2"): #该函数可以返回list下的标签为a的所有信息
    html_ = tag.find('a').get_text()
    print(html_)
words = [] #创建空的列表，存入每章节的url与章节名称
##对标签a内的内容进行提取
for chapter in chapters:
    name = chapter.string  # 取出字符串，可以看出字符串只有章节号与章节名称，刚好符合我们所需
    url1 = url + chapter.get("href")  # 获得每一章节小说的url，可从html代码中看到每一个"href"前边均缺少初始的url，因此需要加上
    word = [url1, name]  # 以列表格式存储
    words.append(word)  # 最终加入总的大列表中并返回
