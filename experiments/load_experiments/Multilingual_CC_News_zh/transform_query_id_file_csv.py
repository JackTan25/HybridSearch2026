import pandas as pd

# 1. CSV
df = pd.read_csv('/home/ubuntu/data_download_data/data_download/dataset/Multilingual_CC-News_zh/Multilingual_CC-News_zh_query.csv')

# 2. query_idquery
df = df[['id', 'text']]

# 3. _idtext
df = df.rename(columns={'id': '_id', 'text': 'text'})

# 4. 
df.to_csv("/home/ubuntu/infinity/experiments/load_experiments/Multilingual_CC_News_zh/queries_with_id.csv", index=False, encoding="utf-8")
