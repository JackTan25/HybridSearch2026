

from enum import Enum

from hybridsearch_embedded.common import hybridsearchException

from hybridsearch_embedded.embedded_hybridsearch_ext import IndexType as LocalIndexType, WrapIndexInfo
from hybridsearch_embedded.embedded_hybridsearch_ext import InitParameter as LocalInitParameter
from hybridsearch_embedded.errors import ErrorCode


class IndexType(Enum):
    IVF = 1
    Hnsw = 2
    FullText = 3
    Secondary = 4
    EMVB = 5
    BMP = 6
    DiskAnn = 7

    def to_local_type(self):
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

    def to_local_type(self):
        local_init_parameter = LocalInitParameter()
        local_init_parameter.param_name = self.param_name
        local_init_parameter.param_value = self.param_value
        return local_init_parameter


class IndexInfo:
    def __init__(self, column_name: str, index_type: IndexType, params: dict = None):
        self.column_name = column_name
        self.index_type = index_type
        if params is not None:
            if isinstance(params, dict):
                self.params = params
            else:
                raise hybridsearchException(ErrorCode.INVALID_INDEX_PARAM, f"{params} should be dictionary type")
        else:
            self.params = None

    def __str__(self):
        return f"IndexInfo({self.column_name}, {self.index_type}, {self.params})"

    def __repr__(self):
        return self.__str__()

    def __eq__(self, other):
        return self.column_name == other.index_name and self.index_type == other.index_type and self.params == other.params

    def __hash__(self):
        return hash((self.column_name, self.index_type, self.params))

    def to_local_type(self):
        index_info_to_use = WrapIndexInfo()
        index_info_to_use.index_type = self.index_type.to_local_type()
        index_info_to_use.column_name = self.column_name.strip()

        index_param_list = []
        if self.params is not None:
            for key, value in self.params.items():
                if isinstance(value, str):
                    local_init_parameter = LocalInitParameter()
                    local_init_parameter.param_name = key
                    local_init_parameter.param_value = value
                    index_param_list.append(local_init_parameter)
                else:
                    raise hybridsearchException(ErrorCode.INVALID_INDEX_PARAM, f"{value} should be string type")

        index_info_to_use.index_param_list = index_param_list
        return index_info_to_use
