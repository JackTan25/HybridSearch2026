import pandas as pd

# 读取CSV文件，假设列名在第一行
df = pd.read_csv("/home/ubuntu/data_download_data/embedding_reserve/mldr_zh/mldr_zh_corpus.csv")

# 重命名列：docid→_id，text→combine_text_and_title
df = df.rename(columns={
    "docid": "_id",
    "text": "combine_text_and_title"
})

# 保存为新文件（不保留索引列）
df.to_csv("/home/ubuntu/data_download_data/embedding_reserve/mldr_zh/mldr_zh_corpus.csv", index=False)
