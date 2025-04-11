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

'''
This example is about connecting to the local hybridsearch instance, creating table, inserting data, and searching data
'''

# import hybridsearch_embedded as hybridsearch
import hybridsearch
import sys
import time

try:
    # Use hybridsearch_embedded module to open a local directory
    # hybridsearch_instance = hybridsearch.connect("/var/hybridsearch")

    #  Use hybridsearch module to connect a remote server
    hybridsearch_instance = hybridsearch.connect(hybridsearch.common.NetworkAddress("127.0.0.1", 23817))

    # 'default_db' is the default database
    db_instance = hybridsearch_instance.get_database("default_db")

    # Drop my_table if it already exists
    db_instance.drop_table("my_table", hybridsearch.common.ConflictType.Ignore)

    # Create a table named "my_table"
    table_instance = db_instance.create_table("my_table", {
        "num": {"type": "integer", "constraints": ["PRIMARY KEY"]},
        "id": {"type": "varchar"},
        "vec": {"type": "vector, 4, float"},
    })

    # Attempt to insert 10 rows
    rows = 10

    start = time.time()
    for idx in range(rows):
        table_instance.insert([
                {
                    "num": idx,
                    "id": "ID_" + str(idx),
                    "vec": [1.0, 1.2, 0.8, 0.9],
                }]
        )

    table_instance.create_index("index1", hybridsearch.index.IndexInfo("id", hybridsearch.index.IndexType.Secondary), hybridsearch.common.ConflictType.Ignore, "secondary_index1")
    res, extra_result = table_instance.filter("id='ID_1'").output(["*"]).to_pl()
    print(res)
    if extra_result is not None:
        print(extra_result)

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
