

from abc import ABC, abstractmethod
from enum import Enum
from typing import Optional, Union, Any

import hybridsearch.remote_thrift.hybridsearch_thrift_rpc.ttypes as ttypes
from hybridsearch.index import IndexInfo
from hybridsearch.common import hybridsearchException, INSERT_DATA
from hybridsearch.errors import ErrorCode


class ExplainType(Enum):
    Analyze = 1
    Ast = 2
    UnOpt = 3
    Opt = 4
    Physical = 5
    Pipeline = 6
    Fragment = 7

    def to_ttype(self):
        if self is ExplainType.Ast:
            return ttypes.ExplainType.Ast
        elif self is ExplainType.Analyze:
            return ttypes.ExplainType.Analyze
        elif self is ExplainType.UnOpt:
            return ttypes.ExplainType.UnOpt
        elif self is ExplainType.Opt:
            return ttypes.ExplainType.Opt
        elif self is ExplainType.Physical:
            return ttypes.ExplainType.Physical
        elif self is ExplainType.Pipeline:
            return ttypes.ExplainType.Pipeline
        elif self is ExplainType.Fragment:
            return ttypes.ExplainType.Fragment
        else:
            raise hybridsearchException(ErrorCode.INVALID_EXPLAIN_TYPE, "Unknown explain type")
