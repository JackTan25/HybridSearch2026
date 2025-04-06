import pandas as pd

# 1. 读取原始CSV文件
df = pd.read_csv('/home/ubuntu/data_download_data/data_download/dataset/mldr_zh/mldr_zh_query.csv')

# 2. 裁剪列：保留query_id和query列
df = df[['query_id', 'query']]

# 3. 修改列名为_id和text
df = df.rename(columns={'query_id': '_id', 'query': 'text'})

# 4. 保存为新文件（不保留行索引）
df.to_csv("/home/ubuntu/infinity/experiments/load_experiments/MLDR_zh/queries_with_id.csv", index=False, encoding="utf-8")
