import os
import requests
from bs4 import BeautifulSoup
import re


def has_class_but_no_id(tag):
    return tag.has_attr('class') and not tag.has_attr('id')


def clean(sign):
    pattern = re.compile(u'[^a-zA-Z]+')  # 中文的范围为\u4e00-\u9fa5 0041-005a,0061-007a  '[^0-9a-zA-Z]+'
    sign = re.sub(pattern, '', str(sign))  # 将其中所有非中文字符替换
    return sign


def get_html():
    htmls = []
    for i in range(2, 170):
        url = "https://novel.tingroom.com/jingdian/list_1_" + str(i) + ".html"
        header = {"User-Agent":
                      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
        req = requests.get(url=url, headers=header)
        req.encoding = "gbk"
        html = req.text
        bes = BeautifulSoup(html, "lxml")
        # texts = bes.find("div", id="list")
        for tag in bes.find_all(class_="yuyu"):  # 该函数可以返回list下的标签为a的所有信息
            html_0 = tag.find('a')
            html_ = html_0.get("href")
            # print(html_)
            htmls.append(html_)
    return htmls


def geturl(html_):
    url = "https://novel.tingroom.com" + html_
    # print(url)
    header = {"User-Agent":
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
    req = requests.get(url=url, headers=header)
    req.encoding = "gbk"
    html = req.text
    bes = BeautifulSoup(html, "lxml")
    words = []
    # print(bes.find_all(class_="clearfix"))
    for text in bes.find_all(has_class_but_no_id, class_="clearfix"):
        chapter = text.find('a')
        # name = chapter.string
        # print(chapter)
        url1 = url + "/" + chapter.get("href")  #
        # print(url1)
        # print(url1)
        # 以列表格式存储
        words.append(url1)  # 最终加入总的大列表中并返回
    return words


if __name__ == '__main__':
    htmls = get_html()
    for i in range(1, 12):
        with open("C:/Users/admin/Documents/text/english" + str(i) + ".txt", "w") as file:  # 写入文件路径 + 章节名称 + 后缀
            for html_ in htmls:
                target = geturl(str(html_))
                htmls.remove(html_)
                end_sign = 0
                header = {"User-Agent":
                              "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"}
                for tar in target:
                    # print(tar)
                    req = requests.get(url=tar, headers=header)
                    req.encoding = 'gbk'
                    html = req.text
                    bes = BeautifulSoup(html, "lxml")
                    # print(bes)
                    texts = bes.find(class_="text").get_text()
                    # print(texts)
                    # texts_list = texts.text.split("\xa0" * 4)
                    # print(texts_list)
                    for line in texts:
                        line = clean(line)
                        print(line)
                        file.write(line.replace(u'\xa0', u''))
                    stats_file = os.path.getsize("C:/Users/admin/Documents/text/english" + str(i) + ".txt")
                    if stats_file > 1024000:
                        end_sign = 1
                        break
                if end_sign == 1:
                    break
