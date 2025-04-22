

import os
import hybridsearch
from tqdm import tqdm
from mldr_common_tools import load_corpus, fvecs_read_yield, read_mldr_sparse_embedding_yield, read_colbert_data_yield
from mldr_common_tools import get_all_part_begin_ends, get_bit_array
import hybridsearch.index as index
from hybridsearch.common import ConflictType, LOCAL_HOST, SparseVector
from hybridsearch.errors import ErrorCode


class hybridsearchClientForInsert:
    def __init__(self):
        self.test_db_name = "default_db"
        self.test_table_name_prefix = "mldr_test_table_text_dense_sparse_colbert_"
        self.test_table_schema = {"docid_col": {"type": "varchar"}, "fulltext_col": {"type": "varchar"},
                                  "dense_col": {"type": "vector,1024,float"},
                                  "sparse_col": {"type": "sparse,250002,float,int"},
                                  "colbert_col": {"type": "tensor,128,float"},
                                  "colbert_bit_col": {"type": "tensor,128,bit"}}
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
        dense_embedding_dir = input("Input dense embedding data files dir: ")
        print("Input begin and end position pairs of dense embedding data to insert:")
        dense_part_begin_ends = get_all_part_begin_ends(total_num)
        sparse_embedding_dir = input("Input sparse embedding data files dir: ")
        print("Input begin and end position pairs of sparse embedding data to insert:")
        sparse_part_begin_ends = get_all_part_begin_ends(total_num)
        colbert_embedding_dir = input("Input colbert embedding data files dir: ")
        print("Input begin and end position pairs of colbert embedding data to insert:")
        colbert_part_begin_ends = get_all_part_begin_ends(total_num)
        insert_num = total_num
        print("Start inserting data...")
        dense_data = None
        dense_pos_part_end = 0
        dense_pair_id_next = 0
        sparse_data = None
        sparse_pos_part_end = 0
        sparse_pair_id_next = 0
        colbert_data = None
        colbert_pos_part_end = 0
        colbert_pair_id_next = 0
        for row_pos in tqdm(range(insert_num)):
            if row_pos == dense_pos_part_end:
                dense_pos_part_begin, dense_pos_part_end = dense_part_begin_ends[dense_pair_id_next]
                dense_pair_id_next += 1
                dense_base_name = f"dense-{dense_pos_part_begin}-{dense_pos_part_end}.fvecs"
                dense_data = fvecs_read_yield(os.path.join(dense_embedding_dir, dense_base_name))
            if row_pos == sparse_pos_part_end:
                sparse_pos_part_begin, sparse_pos_part_end = sparse_part_begin_ends[sparse_pair_id_next]
                sparse_pair_id_next += 1
                sparse_base_name = f"sparse-{sparse_pos_part_begin}-{sparse_pos_part_end}.data"
                sparse_data = read_mldr_sparse_embedding_yield(os.path.join(sparse_embedding_dir, sparse_base_name))
            if row_pos == colbert_pos_part_end:
                colbert_pos_part_begin, colbert_pos_part_end = colbert_part_begin_ends[colbert_pair_id_next]
                colbert_pair_id_next += 1
                colbert_base_name = f"colbert-{colbert_pos_part_begin}-{colbert_pos_part_end}.data"
                colbert_data = read_colbert_data_yield(os.path.join(colbert_embedding_dir, colbert_base_name))
            insert_dense_data = next(dense_data)
            insert_sparse_data = next(sparse_data)
            colbert_list = next(colbert_data)
            insert_dict = {"docid_col": docid_list[row_pos], "fulltext_col": corpus_text_list[row_pos],
                           "dense_col": insert_dense_data, "sparse_col": SparseVector(**insert_sparse_data),
                           "colbert_col": colbert_list, "colbert_bit_col": get_bit_array(colbert_list)}
            self.hybridsearch_table.insert(insert_dict)
        print("Finish inserting data.")
        del dense_data
        del sparse_data
        del colbert_data
        del docid_list
        del corpus_text_list
        print("Start creating fulltext index.")
        ft_params = {}
        if lang == "zh":
            ft_params = {"analyzer": "chinese"}
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
        # print("Start creating EMVB index...")
        # res = self.hybridsearch_table.create_index("emvb_index", index.IndexInfo("colbert_col", index.IndexType.EMVB,
        #                                                                      {"pq_subspace_num": "32",
        #                                                                       "pq_subspace_bits": "8"})
        #                                        , ConflictType.Error)
        # assert res.error_code == ErrorCode.OK
        # print("Finish creating EMVB index.")


if __name__ == "__main__":
    hybridsearch_client = hybridsearchClientForInsert()
    hybridsearch_client.main()
