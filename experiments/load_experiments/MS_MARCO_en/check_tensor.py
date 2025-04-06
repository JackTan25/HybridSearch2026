

import os
import re
import pandas as pd

from MS_MARCO_en.build.colbert_read import load_colbert_list


def extract_number(filename):
    match = re.search(r'(\d+)\.', filename)
    if match:
        return int(match.group(1))
    return 0

def GetQuestions():
    tensor_embedding_dir = "/home/ubuntu/data_download_data/embedding_reserve/MS_MARCO_en/query_tensor_embeddings/vectors"
    # tensor_names = [f for f in os.listdir(tensor_embedding_dir) if os.path.isfile(os.path.join(tensor_embedding_dir, f))]
    # tensor_names = sorted(tensor_names, key=extract_number)
    tensor_file_idx = 0
    questions = 0
    id = 0
    while tensor_file_idx < len(tensor_names):
        # tensor_path = os.path.join(tensor_embedding_dir,tensor_names[tensor_file_idx])
        # tensor_vectors = load_colbert_list(tensor_path)
        questions += len(tensor_vectors)
        tensor_file_idx += 1
        print(tensor_file_idx)
    print(questions)

def main():
    GetQuestions()

if __name__ == "__main__":
    main()