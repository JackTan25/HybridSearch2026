import pandas as pd

# 1. CSV
df = pd.read_csv('/home/ubuntu/data_download_data/data_download/dataset/mldr_zh/mldr_zh_query.csv')

# 2. query_idquery
df = df[['query_id', 'query']]

# 3. _idtext
df = df.rename(columns={'query_id': '_id', 'query': 'text'})

# 4. 
df.to_csv("/home/ubuntu/infinity/experiments/load_experiments/MLDR_zh/queries_with_id.csv", index=False, encoding="utf-8")
