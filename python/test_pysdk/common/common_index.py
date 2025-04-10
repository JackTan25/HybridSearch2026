from enum import Enum

import hybridsearch.remote_thrift.hybridsearch_thrift_rpc.ttypes as ttypes


from hybridsearch.errors import ErrorCode


class IndexType(Enum):
    IVF = 1
    Hnsw = 2
    FullText = 3
    Secondary = 4
    EMVB = 5
    BMP = 6
    DiskAnn = 7

    def to_ttype(self):
        from hybridsearch.common import hybridsearchException
        match self:
            case IndexType.IVF:
                return ttypes.IndexType.IVF
            case IndexType.Hnsw:
                return ttypes.IndexType.Hnsw
            case IndexType.FullText:
                return ttypes.IndexType.FullText
            case IndexType.Secondary:
                return ttypes.IndexType.Secondary
            case IndexType.EMVB:
                return ttypes.IndexType.EMVB
            case IndexType.BMP:
                return ttypes.IndexType.BMP
            case IndexType.DiskAnn:
                return ttypes.IndexType.DiskAnn
            case _:
                raise hybridsearchException(ErrorCode.INVALID_INDEX_TYPE, "Unknown index type")

    def to_local_type(self):
        from hybridsearch_embedded.embedded_hybridsearch_ext import IndexType as LocalIndexType
        from hybridsearch_embedded.common import hybridsearchException
        match self:
            case IndexType.IVF:
                return LocalIndexType.kIVF
            case IndexType.Hnsw:
                return LocalIndexType.kHnsw
            case IndexType.FullText:
                return LocalIndexType.kFullText
            case IndexType.Secondary:
                return LocalIndexType.kSecondary
            case IndexType.EMVB:
                return LocalIndexType.kEMVB
            case IndexType.BMP:
                return LocalIndexType.kBMP
            case IndexType.DiskAnn:
                return LocalIndexType.kDiskAnn
            case _:
                raise hybridsearchException(ErrorCode.INVALID_INDEX_TYPE, "Unknown index type")


class InitParameter:
    def __init__(self, param_name: str, param_value: str):
        self.param_name = param_name
        self.param_value = param_value

    def __str__(self):
        return f"InitParameter({self.param_name}, {self.param_value})"

    def __repr__(self):
        return self.__str__()

    def to_ttype(self):
        return ttypes.InitParameter(self.param_name, self.param_value)

    def to_local_type(self):
        from hybridsearch_embedded.embedded_hybridsearch_ext import InitParameter as LocalInitParameter

        local_init_parameter = LocalInitParameter()
        local_init_parameter.param_name = self.param_name
        local_init_parameter.param_value = self.param_value
        return local_init_parameter
