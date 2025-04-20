import pandas as pd

# CSV
df = pd.read_csv("/home/ubuntu/data_download_data/embedding_reserve/Multilingual_CC_News_zh/Multilingual_CC_News_zh_corpus.csv")

# docid→_idtext→combine_text_and_title
df = df.rename(columns={
    "docid": "_id",
    "text": "combine_text_and_title"
})

# 
df.to_csv("/home/ubuntu/data_download_data/embedding_reserve/Multilingual_CC_News_zh/Multilingual_CC_News_zh_corpus.csv", index=False)
