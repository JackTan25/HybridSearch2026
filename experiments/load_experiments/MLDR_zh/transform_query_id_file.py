from datasets import load_dataset
# 读取JSONL文件
dataset = load_dataset("json", data_files="/home/ubuntu/data_download_data/embedding_reserve/MLDR_zh/queries.jsonl",split="train")
# 保留字段"text"和"label"
dataset = dataset.select_columns(["_id", "text"])
# 保存为CSV（Pandas方案）
import pandas as pd
df = dataset.to_pandas()
df.to_csv("/home/ubuntu/infinity/experiments/load_experiments/MLDR_zh/queries_with_id.csv", index=False, encoding="utf-8")
