import json

with open("/home/ubuntu/infinity/experiments/load_experiments/Multilingual_CC_News_zh/qrels/dev_qrels.jsonl", "r") as f_in, open("/home/ubuntu/infinity/experiments/load_experiments/Multilingual_CC_News_zh/qrels/dev.tsv", "w") as f_out:
    for line in f_in:
        data = json.loads(line)
        f_out.write(f"{data['qid']} {data['docid']} {data['relevance']}\n")