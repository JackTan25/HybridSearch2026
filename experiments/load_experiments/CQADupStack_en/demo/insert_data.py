# Copyright(C) 2023 InfiniFlow, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import re
import time
import os
from tqdm import tqdm
from mldr_common_tools import load_corpus, fvecs_read_yield, read_mldr_sparse_embedding_yield, get_all_part_begin_ends
import hybridsearch
from hybridsearch.common import ConflictType, LOCAL_HOST, SparseVector
import hybridsearch.index as index
from hybridsearch.errors import ErrorCode
from vec_read import load_dense
from vec_read import load_sparse
from colbert_read import load_colbert_list
from datasets import Dataset
# import datasets

def extract_number(filename):
    match = re.search(r'(\d+)\.', filename)
    if match:
        return int(match.group(1))
    return 0

class hybridsearchClientForInsert:
    def __init__(self):
        self.test_db_name = "default_db_demo"
        self.test_table_name_prefix = "CQADupStack_en_Table_demo"
        self.test_table_schema = {"docid_col": {"type": "varchar"}, "fulltext_col": {"type": "varchar"},
                                  "dense_col": {"type": "vector,1024,float16"},
                                  "sparse_col": {"type": "sparse,250002,float16,int"},
                                  "tensor_col": {"type": "tensor,96,float16"}}
        self.hybridsearch_obj = hybridsearch.connect(LOCAL_HOST)
        self.hybridsearch_db = self.hybridsearch_obj.create_database(self.test_db_name, ConflictType.Ignore)
        self.hybridsearch_table = None

    def create_test_table(self):
        table_name = self.test_table_name_prefix
        self.hybridsearch_db.drop_table(table_name, ConflictType.Ignore)
        self.hybridsearch_table = self.hybridsearch_db.create_table(table_name, self.test_table_schema)
        print("Create table successfully.")

    def main(self):
        lang = 'en'
        self.create_test_table()
        corpus = Dataset.from_csv('./CQADupStack_en_combine_corpus.csv')
        total_num = corpus.num_rows
        docid_list = corpus["_id"]
        corpus_text_list = corpus["combine_text_and_title"]
        del corpus
        print(f"Expect total number of rows: {total_num}")
        # dense_embedding_dir = input("Input dense embedding data files dir: ")
        dense_embedding_dir = './small_embedding/CQADupStack_en/dense_embeddings/vectors'
        # print("Input begin and end position pairs of dense embedding data to insert:")
        # dense_part_begin_ends = get_all_part_begin_ends(total_num)
        sparse_embedding_dir = './small_embedding/CQADupStack_en/sparse_embeddings/vectors'
        tensor_embedding_dir = './small_embedding/CQADupStack_en/tensor_embeddings/vectors'
        dense_names = [f for f in os.listdir(dense_embedding_dir) if os.path.isfile(os.path.join(dense_embedding_dir, f))]
        sparse_names = [f for f in os.listdir(sparse_embedding_dir) if os.path.isfile(os.path.join(sparse_embedding_dir, f))]
        tensor_names = [f for f in os.listdir(tensor_embedding_dir) if os.path.isfile(os.path.join(tensor_embedding_dir, f))]
        dense_names = sorted(dense_names, key=extract_number)
        sparse_names = sorted(sparse_names, key=extract_number)
        tensor_names = sorted(tensor_names, key=extract_number)
        # assert(len(dense_names) == len(sparse_names))
        # print("Input begin and end position pairs of sparse embedding data to insert:")
        # sparse_part_begin_ends = get_all_part_begin_ends(total_num)
        # insert_num = int(input("Input insert row count: "))
        insert_num = total_num
        # batch_size = int(input("Input batch size: "))
        batch_size = insert_num/10
        print("Start inserting data...")
        dense_data = None
        # dense_pos_part_end = 0
        # dense_pair_id_next = 0
        sparse_data = None
        # sparse_pos_part_end = 0
        # sparse_pair_id_next = 0
        dense_file_idx = 0
        sparse_file_idx = 0
        tensor_file_idx = 0
        doc_id_idx = 0
        dense_idx = 0
        sparse_idx = 0
        tensor_idx = 0

        sparse_nums = 0
        tensor_nums = 0
        dense_nums = 0
        begin_insert_time = time.time()
        while dense_file_idx < len(dense_names) or sparse_file_idx < len(sparse_names) or tensor_file_idx < len(tensor_names):
            dense_path = os.path.join(dense_embedding_dir,dense_names[dense_file_idx])
            sparse_path = os.path.join(sparse_embedding_dir,sparse_names[sparse_file_idx])
            tensor_path = os.path.join(tensor_embedding_dir,tensor_names[tensor_file_idx])
            dense_vectors = load_dense(dense_path)
            sparse_vectors = load_sparse(sparse_path)
            tensor_vectors = load_colbert_list(tensor_path)
            
            buffer = []
            while dense_idx < len(dense_vectors) and sparse_idx < len(sparse_vectors) and tensor_idx < len(tensor_vectors):
                indices = []
                values = []

                sparse_nums += 1
                tensor_nums += 1
                dense_nums += 1
                for key, value in sparse_vectors[sparse_idx].items():
                    indices.append(int(key))
                    values.append(value)
                insert_dict = {
                                "docid_col": docid_list[doc_id_idx], "fulltext_col": corpus_text_list[doc_id_idx],
                                "dense_col": dense_vectors[dense_idx], "sparse_col": SparseVector(indices,values),
                                "tensor_col": tensor_vectors[tensor_idx],
                               }
                doc_id_idx += 1
                dense_idx += 1
                sparse_idx += 1
                tensor_idx += 1
                buffer.append(insert_dict)
            if doc_id_idx == 1000:
                break
            # print(len(buffer[0]['dense_col']))
            self.hybridsearch_table.insert(buffer[0])
            # break
            if dense_idx >= len(dense_vectors):
                dense_idx = 0
                dense_file_idx += 1
            if sparse_idx >= len(sparse_vectors):
                sparse_idx = 0
                sparse_file_idx += 1
            if tensor_idx >= len(tensor_vectors):
                tensor_idx = 0
                tensor_file_idx += 1
        end_insert_time = time.time()
        print("insert time: ",(end_insert_time - begin_insert_time)*1000,'ms')
        print(f"Finish inserting data. tensor_nums: {tensor_nums+1}, sparse_nums: {sparse_nums+1}, dense_nums: {dense_nums+1}, text_nums: {doc_id_idx+1}")

if __name__ == "__main__":
    hybridsearch_client = hybridsearchClientForInsert()
    hybridsearch_client.main()
