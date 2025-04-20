import datasets
from datasets import Dataset 

def save_to_csv(corpus : datasets.arrow_dataset.Dataset):
    df = corpus.to_pandas()
    #  CSV 
    df.to_csv(f'queries_decline_with_id.csv', index=False)

#  JSON  text 
json_file = "/home/ubuntu/infinity/experiments/small_embedding/CQADupStack_en/english/queries.jsonl"

#  JSON 
def filter_columns(example):
    return {"text": example["text"]}

#  JSON 
dataset = Dataset.from_json(json_file, streaming=True)
filtered_dataset = dataset.map(filter_columns)

#  Dataset
final_dataset = Dataset.from_dict({
    "text": [item["text"] for item in filtered_dataset],
    "id": [item["_id"] for item in filtered_dataset]
})

# 
print(final_dataset)
save_to_csv(final_dataset)
