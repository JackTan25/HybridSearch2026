

"""
This example is to connect local hybridsearch instance, create table, insert data, search the data
"""

# import hybridsearch_embedded as hybridsearch
from multi_client import use_multi_client
import os
import re
import time
from CQADupStack_en.vec_read import load_sparse
import hybridsearch
import sys
from hybridsearch.common import SparseVector
from hybridsearch.common import LOCAL_HOST
from numpy import float16
import pandas as pd
from utils import add_escape_characters

def extract_number(filename):
    match = re.search(r'(\d+)\.', filename)
    if match:
        return int(match.group(1))
    return 0

path_prefix = "/home/ubuntu/hybridsearch/experiments/load_experiments/CQADupStack_en/search"

def remove_extremes_and_average(lst):
    """
    移除列表中的最大值和最小值，返回剩余元素的平均值。
    若列表长度 < 2，返回 0。

    :param lst: 输入列表（支持数值型元素）
    :return: 平均值（浮点数）
    """
    if len(lst) < 2:
        return 0.0  # 或抛出异常，根据需求调整

    # 创建副本避免修改原列表
    filtered = lst.copy()

    # 移除最大值和最小值
    filtered.remove(max(filtered))
    filtered.remove(min(filtered))

    # 处理剩余元素为空的情况
    if not filtered:
        return 0.0

    return sum(filtered) / len(filtered)

cost_time = 0
def sparse_search(hybridsearch_table, question):
     global cost_time
     begin_time = time.time()
     res = (
                hybridsearch_table.output(["docid_col","sparse_col"])
                .match_sparse("sparse_col", question[0], "ip", 10)
                # .to_pl()
            )
     end_time = time.time()
     if use_multi_client == True:
         return None,None
     cost_time += (end_time-begin_time)*1000
     qb_result, extra_result = res.to_pl()
     return qb_result, extra_result

def GetQuestions():
    sparse_embedding_dir = "/home/ubuntu/hybridsearch/experiments/small_embedding/CQADupStack_en/query_sparse_embeddings/vectors"
    df = pd.read_csv('/home/ubuntu/hybridsearch/experiments/small_embedding/CQADupStack_en/english/queries_decline_with_id.csv')
    sparse_names = [f for f in os.listdir(sparse_embedding_dir) if os.path.isfile(os.path.join(sparse_embedding_dir, f))]
    sparse_names = sorted(sparse_names, key=extract_number)
    sparse_file_idx = 0
    questions = []
    id = 0
    while sparse_file_idx < len(sparse_names):
        sparse_path = os.path.join(sparse_embedding_dir,sparse_names[sparse_file_idx])
        query_sparse_vectors = load_sparse(sparse_path)
        for i in range(len(query_sparse_vectors)):
            indices = []
            values = []
            for key, value in query_sparse_vectors[i].items():
                indices.append(int(key))
                values.append(value)
            questions.append((SparseVector(indices,values),df.iloc[id]['id']))
            id += 1
        sparse_file_idx += 1
    return questions

def single_search(questions):
    try:
        #  Use hybridsearch module to connect a remote server
        hybridsearch_instance = hybridsearch.connect(LOCAL_HOST)

        # 'default_db' is the default database
        db_instance = hybridsearch_instance.get_database("default_db")
        hybridsearch_table = db_instance.get_table("CQADupStack_en_Table")
        with open(path_prefix + '/single_road/sparse_result.txt','w') as result_file:
            id = 0
            time_cost = 0
            result_file.write("query-id\tcorpus-id\n")
            for question in questions:
                id += 1
                begin_time = time.time()
                qb_result, extra_result = sparse_search(hybridsearch_table, question)
                end_time = time.time()
                time_cost += (end_time - begin_time) * 1000
                for i in range(len(qb_result['docid_col'])):
                    result_file.write(f"{question[1]}\t{qb_result['docid_col'][i]}\n")
                result_file.flush()
                if extra_result is not None:
                    print(extra_result)
            print("lastid: ",id)
            time_cost = time_cost / len(questions)
            print(f"time_cost: {time_cost} ms")
        hybridsearch_instance.disconnect()
        return time_cost
    except Exception as e:
        print(str(e))
        sys.exit(-1)

if __name__ == "__main__":
    time_cost = []
    questions = GetQuestions()[:1]
    for _ in range(1):
        time_cost.append(single_search(questions))
    print("Average Time Cost: ", remove_extremes_and_average(time_cost))
