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

# import importlib.metadata
#
# __version__ = importlib.metadata.version("hybridsearch_sdk")

import os
# import pkg_resources
# __version__ = pkg_resources.get_distribution("hybridsearch_sdk").version

from hybridsearch_embedded.common import URI, NetworkAddress, LOCAL_HOST, LOCAL_hybridsearch_PATH, hybridsearchException, \
    LOCAL_hybridsearch_CONFIG_PATH
from hybridsearch_embedded.hybridsearch import hybridsearchConnection
from hybridsearch_embedded.local_hybridsearch.hybridsearch import LocalhybridsearchConnection
from hybridsearch_embedded.errors import ErrorCode


def connect(uri=LOCAL_hybridsearch_PATH, config_path=LOCAL_hybridsearch_CONFIG_PATH) -> hybridsearchConnection:
    if isinstance(uri, str) and len(uri) != 0:
        return LocalhybridsearchConnection(uri, config_path)
    else:
        raise hybridsearchException(ErrorCode.INVALID_SERVER_ADDRESS, f"Unknown uri: {uri}")
