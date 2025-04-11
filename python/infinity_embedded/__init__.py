

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
