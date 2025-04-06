import pandas as pd

# 1. 读取原始CSV文件
df = pd.read_csv('/home/ubuntu/data_download_data/data_download/dataset/Multilingual_CC-News_zh/Multilingual_CC-News_zh_query.csv')

# 2. 裁剪列：保留query_id和query列
df = df[['id', 'text']]

# 3. 修改列名为_id和text
df = df.rename(columns={'id': '_id', 'text': 'text'})

# 4. 保存为新文件（不保留行索引）
df.to_csv("/home/ubuntu/infinity/experiments/load_experiments/Multilingual_CC_News_zh/queries_with_id.csv", index=False, encoding="utf-8")
