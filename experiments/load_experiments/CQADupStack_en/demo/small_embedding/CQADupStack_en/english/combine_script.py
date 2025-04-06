import datasets
from datasets import Dataset 
# # 合并 text 和 title，保留 id 和 metadata
# def merge_columns(example):
#     return {
#         "_id": example["_id"],
#         "metadata": example["metadata"],
#         "combine_text_and_title": example["text"] + " " + example["title"]
#     }

# def encode_corpus(corpus: datasets.arrow_dataset.Dataset) -> datasets.arrow_dataset.Dataset:
#     new_dataset = corpus.map(merge_columns, remove_columns=["text", "title"]) 
#     return new_dataset

def save_to_csv(corpus : datasets.arrow_dataset.Dataset):
    df = corpus.to_pandas()
    # 保存为 CSV 文件
    df.to_csv(f'queries_decline.csv', index=False)

# save_to_csv(encode_corpus(Dataset.from_json('/mnt/HDD1/wmz19/cpp_workspace/data_download/dataset/CQADupStack_en/cqadupstack/english/corpus.jsonl')))

# from datasets import Dataset


# from datasets import Dataset

# 从 JSON 文件流式加载，只保留 text 字段
json_file = "/mnt/HDD1/wmz19/cpp_workspace/data_download/dataset/CQADupStack_en/cqadupstack/english/queries.jsonl"

# 流式加载 JSON 数据，只提取需要的字段
def filter_columns(example):
    return {"text": example["text"]}

# 流式加载 JSON 数据
dataset = Dataset.from_json(json_file, streaming=True)
filtered_dataset = dataset.map(filter_columns)

# 将流式数据转换为普通 Dataset
final_dataset = Dataset.from_dict({
    "text": [item["text"] for item in filtered_dataset]
})

# 查看结果
print(final_dataset)
save_to_csv(final_dataset)