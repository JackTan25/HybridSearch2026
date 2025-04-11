# Copyright(C) 2024 InfiniFlow, Inc. All rights reserved.
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


# import hybridsearch_embedded as hybridsearch
import hybridsearch
import sys
import hybridsearch.index as index
from hybridsearch.common import ConflictType, LOCAL_HOST, SparseVector
from hybridsearch.errors import ErrorCode

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
        "num": {"type": "integer"},
        "body": {"type": "varchar"},
        "vec": {"type": "tensor,4,float"},
    })

    # Insert 4 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "body": r"unnecessary and harmful",
                "vec": [[1.0, 0.0, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]],
            },
            {
                "num": 2,
                "body": r"Office for Harmful Blooms",
                "vec": [[4.0, 0.0, 4.3, 4.5], [4.0, 4.2, 4.4, 5.0]],
            },
            {
                "num": 3,
                "body": r"A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
                "vec": [[0.9, 0.1, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]],
            },
            {
                "num": 4,
                "body": r"The American Football Conference (AFC) harm chemical anarchism add test is one of harm chemical the two conferences of the National Football League (NFL). This add test conference and its counterpart, the National Football Conference (NFC), currently contain 16 teams each, making up the 32 teams of the NFL. The current AFC title holder is the New England Patriots.",
                "vec": [[4.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
            },
        ]
    )
    res = table_instance.create_index("tensor_index", index.IndexInfo("vec", index.IndexType.EMVB,
                                {
                                    "pq_subspace_num":"1",
                                    "pq_subspace_bits":"8",
                                }
                            ),
                        ConflictType.Error)
    result, extra_result = table_instance.output(["num", "vec", "_score"]).match_tensor("vec",
                                                                                        [[0.9, 0.0, 0.0, 0.0],
                                                                                         [1.1, 0.0, 0.0, 0.0]],
                                                                                        'float', 2).to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
