import numpy as np
import pandas as pd

qrel_file_path = "/home/ubuntu/hybridsearch/experiments/small_embedding/CQADupStack_en/english/qrels/test.tsv"
query_result_file_path = "/home/ubuntu/hybridsearch/experiments/load_experiments/CQADupStack_en/search_weigthed_sum100/three_roads/fulltext_dense_sparse_result.txt"

def load_qrel_map():
    """
    加载 qrel 文件，构建 query-id 到 corpus-id 到 score 的映射
    :return: query-id 到 corpus-id 到 score 的映射
    """
    # 读取 qrel 文件
    df = pd.read_csv(qrel_file_path, sep='\t')

    # 按照 query-id 进行分组
    grouped = df.groupby('query-id')

    # 创建一个字典来存储每个 query-id 对应的 corpus
    result = {}
    for query_id, group in grouped:
        # 将 corpus-id 作为键，score 作为值，构建映射
        mapping = dict(zip(group['corpus-id'], group['score']))
        result[query_id] = mapping
    return result

def calculate_dcg(scores, k=None):
    """
    计算折损累积增益(DCG)

    :param scores: 相关性得分的数组
    :param k: 计算 DCG 的前 k 个结果，若为 None 则计算整个数组的 DCG
    :return: DCG 值
    """
    if k is None:
        k = len(scores)
    if k > len(scores):
        k = len(scores)
    # 取前 k 个得分
    scores = scores[:k]
    # 计算 DCG
    dcg = (2 ** scores[0]) - 1  # 第一个位置使用标准公式计算
    for i in range(1, len(scores)):
        dcg += ((2 ** scores[i]) - 1) / np.log2(i + 2)
    return dcg

def main():
    qrel_map = load_qrel_map()
    df = pd.read_csv(query_result_file_path, sep='\t')
    grouped = df.groupby('query-id')
    result = {}
    for query_id, group in grouped:
        # 将每个组内的 corpus-id 按原始顺序存储在列表中
        corpus_ids = group['corpus-id'].tolist()
        result[query_id] = corpus_ids
    result_ndcg = 0
    for query_id, corpus_id_list in result.items():
        # 获取当前 query-id 对应的 qrel 映射
        qrel_mapping = qrel_map.get(query_id, {})
        # 获取当前 query-id 对应的 corpus
        scores_list = [qrel_mapping.get(corpus_id, 0) for corpus_id in corpus_id_list]
        dcg = calculate_dcg(scores_list, 100)
        idcg = calculate_dcg(sorted(scores_list, reverse=True), 100)
        ndcg = dcg / idcg if idcg != 0 else 0
        print(f"Query {query_id} NDCG@10: {ndcg}")
        result_ndcg += ndcg
    print(result_ndcg / len(result))

if __name__ == '__main__':
   main()
