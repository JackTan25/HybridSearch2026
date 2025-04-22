

import os
from tqdm import tqdm
from experiments.load_experiments.CQADupStack_en.build.mldr_common_tools import load_corpus, fvecs_read_yield, read_mldr_sparse_embedding_yield, get_all_part_begin_ends
import hybridsearch
from hybridsearch.common import ConflictType, LOCAL_HOST, SparseVector
import hybridsearch.index as index
from hybridsearch.errors import ErrorCode
from experiments.load_experiments.CQADupStack_en.build.vec_read import load_dense
from experiments.load_experiments.CQADupStack_en.build.vec_read import load_sparse

class hybridsearchClientForInsert:
    def __init__(self):
        self.test_db_name = "default_db"
        self.test_table_name_prefix = "mldr_test_table_text_dense_sparse_"
        self.test_table_schema = {"docid_col": {"type": "varchar"}, "fulltext_col": {"type": "varchar"},
                                  "dense_col": {"type": "vector,1024,float"},
                                  "sparse_col": {"type": "sparse,250002,float,int"}}
        self.hybridsearch_obj = hybridsearch.connect(LOCAL_HOST)
        self.hybridsearch_db = self.hybridsearch_obj.create_database(self.test_db_name, ConflictType.Ignore)
        self.hybridsearch_table = None

    def create_test_table(self, language_suffix: str):
        table_name = self.test_table_name_prefix + language_suffix
        self.hybridsearch_db.drop_table(table_name, ConflictType.Ignore)
        self.hybridsearch_table = self.hybridsearch_db.create_table(table_name, self.test_table_schema)
        print("Create table successfully.")

    def main(self):
        lang = input("Input language to insert: ")
        self.create_test_table(lang)
        corpus = load_corpus(lang)
        total_num = corpus.num_rows
        docid_list = corpus["docid"]
        corpus_text_list = corpus["text"]
        del corpus
        print(f"Expect total number of rows: {total_num}")
        # dense_embedding_dir = input("Input dense embedding data files dir: ")
        dense_embedding_dir = '/home/ubuntu/experiments/small_embedding/CQADupStack_en/dense_embeddings/vectors'
        # print("Input begin and end position pairs of dense embedding data to insert:")
        # dense_part_begin_ends = get_all_part_begin_ends(total_num)
        sparse_embedding_dir = '/home/ubuntu/experiments/small_embedding/CQADupStack_en/sparse_embeddings/vectors'
        dense_names = [f for f in os.listdir(dense_embedding_dir) if os.path.isfile(os.path.join(dense_embedding_dir, f))]
        sparse_names = [f for f in os.listdir(sparse_embedding_dir) if os.path.isfile(os.path.join(sparse_embedding_dir, f))]
        assert(len(dense_names == sparse_names))
        # print("Input begin and end position pairs of sparse embedding data to insert:")
        # sparse_part_begin_ends = get_all_part_begin_ends(total_num)
        # insert_num = int(input("Input insert row count: "))
        insert_num = total_num
        # batch_size = int(input("Input batch size: "))
        batch_size = insert_num/10
        print("Start inserting data...")
        dense_data = None

        sparse_data = None

        dense_file_idx = 0
        sparse_file_idx = 0
        doc_id_idx = 0
        dense_idx = 0
        sparse_idx = 0
        while dense_file_idx < len(dense_names) and sparse_file_idx < len(sparse_names):
            dense_path = os.path.join(dense_embedding_dir,dense_names[dense_file_idx])
            sparse_path = os.path.join(sparse_embedding_dir,sparse_names[sparse_file_idx])
            dense_vectors = load_dense(dense_path)
            sparse_vectors = load_sparse(sparse_path)
            buffer = []
            while dense_idx < len(dense_vectors) and sparse_idx < len(sparse_vectors):
                insert_dict = {"docid_col": docid_list[doc_id_idx], "fulltext_col": corpus_text_list[doc_id_idx],
                               "dense_col": dense_vectors[dense_idx], "sparse_col": SparseVector(sparse_vectors[sparse_idx])}
                doc_id_idx += 1
                dense_idx += 1
                sparse_idx += 1
                buffer.append(insert_dict)
            self.hybridsearch_table.insert(buffer)
            if dense_idx >= len(dense_vectors):
                dense_idx = 0
                dense_file_idx += 1
            if sparse_idx >= len(sparse_vectors):
                sparse_idx = 0
                sparse_file_idx += 1

        # for begin_idx in tqdm(range(0, insert_num, batch_size)):
        #     end_idx = min(begin_idx + batch_size, insert_num)
        #     buffer = []
        #     for row_pos in range(begin_idx, end_idx):
        #         if row_pos == dense_pos_part_end:
        #             dense_pos_part_begin, dense_pos_part_end = dense_part_begin_ends[dense_pair_id_next]
        #             dense_pair_id_next += 1
        #             dense_base_name = f"dense-{dense_pos_part_begin}-{dense_pos_part_end}.fvecs"
        #             dense_data = fvecs_read_yield(os.path.join(dense_embedding_dir, dense_base_name))
        #         if row_pos == sparse_pos_part_end:
        #             sparse_pos_part_begin, sparse_pos_part_end = sparse_part_begin_ends[sparse_pair_id_next]
        #             sparse_pair_id_next += 1
        #             sparse_base_name = f"sparse-{sparse_pos_part_begin}-{sparse_pos_part_end}.data"
        #             sparse_data = read_mldr_sparse_embedding_yield(os.path.join(sparse_embedding_dir, sparse_base_name))
        #         insert_dict = {"docid_col": docid_list[row_pos], "fulltext_col": corpus_text_list[row_pos],
        #                        "dense_col": next(dense_data), "sparse_col": SparseVector(**(next(sparse_data)))}
        #         buffer.append(insert_dict)
        #     self.hybridsearch_table.insert(buffer)
        #     buffer.clear()
        print("Finish inserting data.")
        del dense_data
        del sparse_data
        del docid_list
        del corpus_text_list
        print("Start creating fulltext index.")
        ft_params = []
        if lang == "zh":
            ft_params.append(index.InitParameter("analyzer", "chinese"))
        res = self.hybridsearch_table.create_index("ft_index",
                                               index.IndexInfo("fulltext_col", index.IndexType.FullText, ft_params),
                                               ConflictType.Error)
        assert res.error_code == ErrorCode.OK
        print("Finish creating fulltext index.")
        print("Start creating Hnsw index...")
        res = self.hybridsearch_table.create_index("hnsw_index", index.IndexInfo("dense_col", index.IndexType.Hnsw,
                                                                             {
                                                                                 "m": "16",
                                                                                 "ef_construction": "200",
                                                                                 "metric": "ip",
                                                                                 "encode": "lvq"
                                                                             }),
                                               ConflictType.Error)
        assert res.error_code == ErrorCode.OK
        print("Finish creating Hnsw index.")
        print("Start creating BMP index...")
        res = self.hybridsearch_table.create_index("bmp_index", index.IndexInfo("sparse_col", index.IndexType.BMP,
                                                                            {
                                                                                "block_size": "8",
                                                                                "compress_type": "compress"
                                                                            }),
                                               ConflictType.Error)
        assert res.error_code == ErrorCode.OK
        self.hybridsearch_table.optimize("bmp_index", {"topk": "1000", "bp_reorder": ""})
        print("Finish creating BMP index.")


if __name__ == "__main__":
    hybridsearch_client = hybridsearchClientForInsert()
    hybridsearch_client.main()
