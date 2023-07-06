from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.naive_bayes import MultinomialNB
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
import os
import glob

def get_corpus():
    corpus = []
    labels = []
    edu_path = "C:\\Users\\admin\\Desktop\\my_repo\\hw_nlp4\\passage\\edu"
    ent_path = "C:\\Users\\admin\\Desktop\\my_repo\\hw_nlp4\\passage\\ent"
    fashion_path = "C:\\Users\\admin\\Desktop\\my_repo\\hw_nlp4\\passage\\fashion"
    sports_path = "C:\\Users\\admin\\Desktop\\my_repo\\hw_nlp4\\passage\\sports"
    tech_path = "C:\\Users\\admin\\Desktop\\my_repo\\hw_nlp4\\passage\\tech"
    file_paths = glob.glob(os.path.join(edu_path, '*'))
    file_count = len(file_paths)
    # print(file_paths)
    for file in file_paths:
        with open(file, "r",encoding="utf-8") as f:
            corpus.append(f.read())
            labels.append("edu")

    file_paths = glob.glob(os.path.join(ent_path, '*'))
    file_count = len(file_paths)
    # print(file_paths)
    for file in file_paths:
        with open(file, "r",encoding="utf-8") as f:
            corpus.append(f.read())
            labels.append("ent")

    file_paths = glob.glob(os.path.join(fashion_path, '*'))
    file_count = len(file_paths)
    # print(file_paths)
    for file in file_paths:
        with open(file, "r",encoding="utf-8") as f:
            corpus.append(f.read())
            labels.append("fashion")

    file_paths = glob.glob(os.path.join(tech_path, '*'))
    file_count = len(file_paths)
    # print(file_paths)
    i=0
    for file in file_paths:
        if i>=25:
            break
        i+=1
        with open(file, "r",encoding="utf-8") as f:
            corpus.append(f.read())
            labels.append("tech")
    
    return corpus, labels