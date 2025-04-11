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

"""
This example is to connect local hybridsearch instance, create table, insert data, search the data
"""

# import hybridsearch_embedded as hybridsearch
import os
from multi_client import use_multi_client
import time
import hybridsearch
import sys

from hybridsearch.common import LOCAL_HOST
import pandas as pd
from utils import add_escape_characters

path_prefix = "/home/ubuntu/hybridsearch/experiments/load_experiments/Touche_2020_en/search"

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

import subprocess
import time


import psutil
def terminate_process_tree(pid):
    try:
        parent = psutil.Process(pid)
        for child in parent.children(recursive=True):
            child.terminate()
        parent.terminate()
    except psutil.NoSuchProcess:
        pass

def read_file_content(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
            return content
    except FileNotFoundError:
        print(f"错误：文件 {file_path} 未找到。")
    except Exception as e:
        print(f"错误：发生未知错误 - {e}")
    return None

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
    df = pd.read_csv('/home/ubuntu/hybridsearch/experiments/load_experiments/Touche_2020_en/queries_with_id.csv')
    questions = []
    for _, row in df.iterrows():
        questions.append((add_escape_characters(row['text']).strip(),row['_id']))
    return questions

def single_search(questions):
    try:
        #  Use hybridsearch module to connect a remote server
        hybridsearch_instance = hybridsearch.connect(LOCAL_HOST)

        # 'default_db' is the default database
        db_instance = hybridsearch_instance.get_database("default_db")
        hybridsearch_table = db_instance.get_table("Touche_2020_en_Table")
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
    # 启动服务并在适当的时候杀死它
    service_command = "/home/ubuntu/hybridsearch/cmake-build-release/src/hybridsearch -f /home/ubuntu/hybridsearch/conf/hybridsearch_conf.toml"  # 示例命令，启动一个简单的 HTTP 服务器
    process = subprocess.Popen(service_command, shell=True)
    time.sleep(3)
    print(f"服务已启动，进程 ID: {process.pid}")
    print(__file__)
    time_cost = []
    questions = GetQuestions()
    for _ in range(1):
        time_cost.append(single_search(questions))
    print("Average Time Cost: ", remove_extremes_and_average(time_cost)," ms")
    current_dir = os.path.dirname(os.path.abspath(__file__))
    current_file_path = __file__
    current_file_name = os.path.basename(current_file_path)
    current_file_name_without_extension = os.path.splitext(current_file_name)[0]
    with open(current_dir + "/" + current_file_name_without_extension + ".time",'w') as tfile:
        tfile.write(f"{cost_time/len(questions)} ms")
        tfile.flush()
    # 读取文件内容
    file_path = '/home/ubuntu/hybridsearch/experiments/query_memory_file'
    content = read_file_content(file_path)
    with open(current_dir + "/" + current_file_name_without_extension + ".memory",'w') as mfile:
        mfile.write(content)
        mfile.flush()
    terminate_process_tree(process.pid)
    print(f"服务进程 {process.pid} 已被终止。")
