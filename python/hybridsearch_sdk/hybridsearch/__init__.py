

# import importlib.metadata
#
# __version__ = importlib.metadata.version("hybridsearch_sdk")

import logging
import os
# import pkg_resources
# __version__ = pkg_resources.get_distribution("hybridsearch_sdk").version

from hybridsearch.common import URI, NetworkAddress, LOCAL_HOST, LOCAL_hybridsearch_PATH, hybridsearchException
from hybridsearch.hybridsearch import hybridsearchConnection
from hybridsearch.remote_thrift.hybridsearch import RemoteThrifthybridsearchConnection
from hybridsearch.errors import ErrorCode


def connect(uri=LOCAL_HOST, logger: logging.Logger = None) -> hybridsearchConnection:
    if isinstance(uri, NetworkAddress):
        return RemoteThrifthybridsearchConnection(uri, logger)
    else:
        raise hybridsearchException(ErrorCode.INVALID_SERVER_ADDRESS, f"Unknown uri: {uri}")
