import datasets
from datasets import Dataset 
import pandas as pd

# 读取 CSV 文件
file_path = '/home/ubuntu/infinity/experiments/load_experiments/MLDR_en/search/mldr_en_query.csv'
df = pd.read_csv(file_path)

# 指定需要保存的列名
columns_to_keep = ['query_id', 'query']

# 筛选出特定的列
filtered_df = df[columns_to_keep]

# 将筛选后的数据保存到新的 CSV 文件
output_file = 'queries_decline_with_id.csv'
filtered_df.to_csv(output_file, index=False)