import numpy as np
import math

txt_number = 11
total_number = 0
dictionary = np.zeros(50000)
for i in range(1, txt_number+1):
    max_entropy = 0
    # print(i)
    file = open("C:/Users/admin/Documents/text/english" + str(i) + ".txt")
    word = '1'
    while len(word) != 0:
        word = file.read(1)
        if word == '\n' or len(word) == 0:
            continue
        total_number = total_number + 1
        word_unicode = ord(word.lower())  # unicode
        dictionary[word_unicode] += 1
    _dictionary = dictionary/total_number
    for number_ in range(1, 50000):
        p = _dictionary[number_]
        if p != 0:
            max_entropy -= p * math.log(p, 2)
    print("while the size of document is ", i, "KB, the maximum-entropy=", max_entropy)
    file.close()
