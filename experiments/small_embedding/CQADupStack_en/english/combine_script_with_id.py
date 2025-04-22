import datasets
from datasets import Dataset 

def save_to_csv(corpus : datasets.arrow_dataset.Dataset):
    df = corpus.to_pandas()
    # 保存为 CSV 文件
    df.to_csv(f'queries_decline_with_id.csv', index=False)

# 从 JSON 文件流式加载，只保留 text 字段
json_file = "/home/ubuntu/hybridsearch/experiments/small_embedding/CQADupStack_en/english/queries.jsonl"

# 流式加载 JSON 数据，只提取需要的字段
def filter_columns(example):
    return {"text": example["text"]}

# 流式加载 JSON 数据
dataset = Dataset.from_json(json_file, streaming=True)
filtered_dataset = dataset.map(filter_columns)

# 将流式数据转换为普通 Dataset
final_dataset = Dataset.from_dict({
    "text": [item["text"] for item in filtered_dataset],
    "id": [item["_id"] for item in filtered_dataset]
})

# 查看结果
print(final_dataset)
save_to_csv(final_dataset)
