
import os
import requests
from bs4 import BeautifulSoup
import re


def clean(sign):
    pattern = re.compile(u'[^\u4e00-\u9fa5]') #中文的范围为\u4e00-\u9fa5
    sign = re.sub(pattern,'\n',str(sign)) #将其中所有非中文字符替换
    return sign

def get_html():
    url = "https://www.xqb5200.com/"
    header = {"User-Agent":
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
    req = requests.get(url=url, headers=header)
    req.encoding = "gbk"
    html = req.text
    bes = BeautifulSoup(html, "lxml")
    # texts = bes.find("div", id="list")
    htmls = []
    for tag in bes.find_all(class_="s2"):  # 该函数可以返回list下的标签为a的所有信息
        html_0 = tag.find('a')
        html_ = html_0.get("href")
        # print(html_)
        htmls.append(html_)
    return htmls


def geturl(html_):
    url = html_
    header = {"User-Agent":
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
    req = requests.get(url = url, headers = header)
    req.encoding = "gbk"
    html = req.text
    bes = BeautifulSoup(html,"lxml")
    texts = bes.find("div", id="list")
    chapters = texts.find_all("a") #该函数可以返回list下的标签为a的所有信息
    words = [] #创建空的列表，存入每章节的url与章节名称
    ##对标签a内的内容进行提取
    # print(chapters)
    for chapter in chapters:
        name = chapter.string #取出字符串，可以看出字符串只有章节号与章节名称，刚好符合我们所需
        # print(chapter)
        url1 = url + chapter.get("href") #获得每一章节小说的url，可从html代码中看到每一个"href"前边均缺少初始的url，因此需要加上
        # print(url1)
        word = [url1, name] #以列表格式存储
        words.append(word) #最终加入总的大列表中并返回
    return words

if __name__ == '__main__':
    htmls = get_html()
    for i in range(1,12):
        with open("C:/Users/admin/Documents/text/chinese" + str(i) + ".txt", "w") as file:# 写入文件路径 + 章节名称 + 后缀
            for html_ in htmls:
                target = geturl(str(html_))
                htmls.remove(html_)
                end_sign = 0
                header = {"User-Agent":
                              "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
                for tar in target:
                    req = requests.get(url=tar[0], headers=header)
                    req.encoding = 'gbk'
                    html = req.text
                    bes = BeautifulSoup(html, "lxml")
                    texts = bes.find("div", id="content")
                    texts_list = texts.text.split("\xa0" * 4)
                    print(texts_list)
                    for line in texts_list:
                        line = clean(line)
                        file.write(line.replace(u'\xa0', u''))
                    stats_file = os.path.getsize("C:/Users/admin/Documents/text/chinese" + str(i) + ".txt")
                    if stats_file > 1024000:
                        end_sign = 1
                        break
                if end_sign == 1:
                    break






