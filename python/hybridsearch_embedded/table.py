

from enum import Enum

from hybridsearch_embedded.common import hybridsearchException, INSERT_DATA
from hybridsearch_embedded.embedded_hybridsearch_ext import ExplainType as LocalExplainType
from hybridsearch_embedded.errors import ErrorCode


class ExplainType(Enum):
    Analyze = 1
    Ast = 2
    UnOpt = 3
    Opt = 4
    Physical = 5
    Pipeline = 6
    Fragment = 7

    def to_local_ttype(self):
        if self is ExplainType.Ast:
            return LocalExplainType.kAst
        elif self is ExplainType.Analyze:
            return LocalExplainType.kAnalyze
        elif self is ExplainType.UnOpt:
            return LocalExplainType.kUnOpt
        elif self is ExplainType.Opt:
            return LocalExplainType.kOpt
        elif self is ExplainType.Physical:
            return LocalExplainType.kPhysical
        elif self is ExplainType.Pipeline:
            return LocalExplainType.kPipeline
        elif self is ExplainType.Fragment:
            return LocalExplainType.kFragment
        else:
            raise hybridsearchException(ErrorCode.INVALID_EXPLAIN_TYPE, "Unknown explain type")
