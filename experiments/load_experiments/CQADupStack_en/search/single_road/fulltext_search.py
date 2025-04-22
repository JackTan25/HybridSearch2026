

"""
This example is to connect local hybridsearch instance, create table, insert data, search the data
"""

# import hybridsearch_embedded as hybridsearch
from multi_client import use_multi_client
import os
import time
import hybridsearch
import sys

from hybridsearch.common import LOCAL_HOST
import pandas as pd
from utils import add_escape_characters

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
def fulltext_search(hybridsearch_table, question):
     global cost_time
     begin_time = time.time()
     res = (
                hybridsearch_table.output(["docid_col","fulltext_col"])
                .match_text("fulltext_col", question[0], 10)
                # .to_pl()
            )
     end_time = time.time()
     if use_multi_client == True:
         return None,None
     cost_time += (end_time-begin_time)*1000
     qb_result, extra_result = res.to_pl()
     return qb_result, extra_result

def GetQuestions():
    df = pd.read_csv('/home/ubuntu/hybridsearch/experiments/small_embedding/CQADupStack_en' + '/english/queries_decline_with_id.csv')
    questions = []
    for _, row in df.iterrows():
        questions.append((add_escape_characters(row['text']).strip(),row['id']))
    return questions

def single_search(questions):
    try:
        #  Use hybridsearch module to connect a remote server
        hybridsearch_instance = hybridsearch.connect(LOCAL_HOST)

        # 'default_db' is the default database
        db_instance = hybridsearch_instance.get_database("default_db")
        hybridsearch_table = db_instance.get_table("CQADupStack_en_Table")
        with open(path_prefix + '/single_road/fulltext_result.txt','w') as result_file:
            id = 0
            time_cost = 0
            result_file.write("query-id\tcorpus-id\n")
            for question in questions:
                id += 1
                begin_time = time.time()
                qb_result, extra_result = fulltext_search(hybridsearch_table, question)
                end_time = time.time()
                time_cost += (end_time - begin_time) * 1000
                for i in range(len(qb_result['docid_col'])):
                    result_file.write(f"{question[1]}\t{qb_result['docid_col'][i]}\n")
                result_file.flush()
                if extra_result is not None:
                    print(extra_result)
            time_cost = time_cost / len(questions)
            print(f"time_cost: {time_cost} ms")
        hybridsearch_instance.disconnect()
        return time_cost
    except Exception as e:
        print(str(e))
        sys.exit(-1)

if __name__ == "__main__":
    time_cost = []
    questions = GetQuestions()
    for _ in range(1):
        time_cost.append(single_search(questions))
    print("Average Time Cost: ", remove_extremes_and_average(time_cost))
    current_dir = os.path.dirname(os.path.abspath(__file__))
    current_file_path = __file__
    current_file_name = os.path.basename(current_file_path)
    current_file_name_without_extension = os.path.splitext(current_file_name)[0]
    with open(current_dir + "/" + current_file_name_without_extension + ".time",'w') as tfile:
        tfile.write(f"{cost_time/len(questions)} ms")
        tfile.flush()