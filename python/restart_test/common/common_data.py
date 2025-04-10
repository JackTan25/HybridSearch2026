from hybridsearch.common import ConflictType, hybridsearchException
from hybridsearch import index
from numpy import dtype
import ast
from enum import Enum

default_url = "http://localhost:23820/"

baseHeader = {
    'accept': 'application/json',
    'content-type': 'application/json',
}

baseData = {
    "create_option": "ignore_if_exists",
    "drop_option": "ignore_if_not_exists",
    "fields": {},
    "properties": {}
}

types = [
    "integer", "tinyint", "smallint", "bigint", "hugeint", "float",
    "double", "varchar", "boolean"
]

unsupport_output = ["_similarity", "_row_id", "_score", "_distance"]

type_transfrom = {
    "int":"integer",
    "uint8":"uint8",
    "int8":"tinyint",
    "int16":"smallint",
    "int32":"integer",
    "integer":"integer",
    "int64":"bigint", # hugeint unsupport
    "float16":"float16",
    "bfloat16":"bfloat16",
    "float":"float",
    "float32":"float",
    "float64":"double",
    "double":"double",
    "varchar":"varchar",
    "bool":"boolean",

    "INT":"INTEGER",
    "FLOAT":"FLOAT",
}

def type_to_dtype(type):
    match type.lower():
        case "bool":
            return dtype('bool')
        case "boolean":
            return dtype('bool')
        case "tinyint":
            return dtype('int8')
        case "smallint":
            return dtype('int16')
        case "integer":
            return dtype('int32')
        case "int":
            return dtype('int32')
        case "int32":
            return dtype('int32')
        case "bigint":
            return dtype('int64')
        case "float":
            return dtype('float32')
        case "float32":
            return dtype('float32')
        case "float64":
            return dtype('float64')
        case "float16":
            return dtype('float32')
        case "bfloat16":
            return dtype('float32')
        case "double":
            return dtype('float64')
        case "varchar":
            return dtype('str')
        case _:
            return object

def is_float(str):
    try:
        float(str)
        return True
    except:
        return False

def is_list(str):
    try:
        result = ast.literal_eval(str)
        return isinstance(result, list)
    except:
        return False

def is_bool(str):
    return str.lower() == "true" or str.lower() == "false"

index_type_transfrom = {
    index.IndexType.IVF:"IVF",
    index.IndexType.Hnsw:"HNSW",
    index.IndexType.FullText:"FULLTEXT",
    index.IndexType.Secondary:"Secondary",
    index.IndexType.BMP:"BMP",
    index.IndexType.EMVB:"EMVB",
}

baseResponse = {
    "error_code": 0,
}

baseCreateOptions = {
    ConflictType.Ignore: "ignore_if_exists",
    ConflictType.Error: 'error',
    ConflictType.Replace: "replace_if_exists"
}

baseDropOptions = {
    ConflictType.Ignore: "ignore_if_not_exists",
    ConflictType.Error: "error",
}

tableDefaultData = {
    "fields":
        {
            "name":
                {
                    "type": "varchar",
                    "constraints": ["not null"]
                },
            "age":
                {
                    "type": "integer",
                    "constraints": ["not null"]
                },
            "score":
                {
                    "type": "integer",
                    "constraints": ["not null"]
                }
        },
    "properties":
        {
            "bloomfilter_columns":
                {
                    "age",
                    "score"
                }
        }
}


'''
enum class LiteralType : int32_t {
    kBoolean,
    kDouble,
    kString,
    kInteger,
    kNull,
    kDate,
    kTime,
    kDateTime,
    kTimestamp,
    kIntegerArray,
    kDoubleArray,
    kSubArrayArray,
    kInterval,
    kLongSparseArray,
    kDoubleSparseArray,
    kEmptyArray,
};
'''
class literaltype(Enum):
    kBoolean = 0
    kDouble = 1
    kString = 2
    kInteger = 3
    kNull = 4
    kDate = 5
    kTime= 6
    kDateTime = 7
    kTimestamp = 8
    kIntegerArray = 9
    kDoubleArray = 10
    kSubArrayArray = 11
    kInterval = 12
    kLongSparseArray = 13
    kDoubleSparseArray = 14
    kEmptyArray = 15

type_to_literaltype = {
    "boolean" : literaltype.kBoolean.value,
    "tinyint" : literaltype.kInteger.value,
    "smallint" : literaltype.kInteger.value,
    "int" : literaltype.kInteger.value,
    "integer" : literaltype.kInteger.value,
    "bigint" : literaltype.kInteger.value,
    "float" : literaltype.kDouble.value,
    "double": literaltype.kDouble.value
}

type_to_vector_literaltype = {
    "integer": literaltype.kIntegerArray.value,
    "float": literaltype.kDoubleArray.value,
    "double": literaltype.kDoubleArray.value
}
