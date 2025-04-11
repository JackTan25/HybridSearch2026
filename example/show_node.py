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

try:
    # Use hybridsearch_embedded module to open a local directory
    # hybridsearch_instance = hybridsearch.connect("/var/hybridsearch")

    #  Use hybridsearch module to connect a remote server
    hybridsearch_instance = hybridsearch.connect(hybridsearch.common.NetworkAddress("127.0.0.1", 23817))

    res = hybridsearch_instance.show_current_node()
    print(res)

    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
