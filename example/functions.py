import os
import pandas as pd
import hybridsearch
import hybridsearch.index as index
from numpy import dtype
from hybridsearch.errors import ErrorCode
from hybridsearch.common import ConflictType, SortType
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)


hybridsearch_obj = hybridsearch.connect(hybridsearch.common.NetworkAddress("127.0.0.1", 23817))
db_obj = hybridsearch_obj.get_database("default_db")
db_obj.drop_table("function_example", ConflictType.Ignore)
db_obj.create_table("function_example",
                    {"c1": {"type": "varchar", "constraints": ["primary key", "not null"]},
                        "c2": {"type": "varchar", "constraints": ["not null"]}}, ConflictType.Error)
table_obj = db_obj.get_table("function_example")
table_obj.insert(
    [{"c1": 'a', "c2": 'a'}, {"c1": 'b', "c2": 'b'}, {"c1": 'c', "c2": 'c'}, {"c1": 'd', "c2": 'd'},
        {"c1": 'abc', "c2": 'abc'}, {"c1": 'bbc', "c2": 'bbc'}, {"c1": 'cbc', "c2": 'cbc'}, {"c1": 'dbc', "c2": 'dbc'},
        {"c1": 'abcd', "c2": 'abc'}, 
        {"c1": '', "c2": 'email'}, {"c1": 'test@hotmail.com', "c2": 'email'},
        {"c1": ' abc', "c2": 'abc'}, {"c1": 'abc ', "c2": 'abc'}, {"c1": ' abc ', "c2": 'abc'}])

# varchar functions

#function char_length
res, extra_result = table_obj.output(["*", "char_length(c1)"]).filter("char_length(c1) = 1").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "char_length(c1)"]).filter("char_length(c1) = 3").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "char_length(c1)"]).filter("char_length(c1) = 4").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "char_length(c1)"]).filter("char_length(c1) = char_length(c2)").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function regex
res, extra_result = table_obj.output(["*", "regex(c1, 'bc')"]).filter("regex(c1, 'bc')").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*"]).filter("regex(c1, '(\w+([-+.]\w+)*)@(\w+([-.]\w+)*)\.(\w+([-.]\w+)*)')").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function substring
res, extra_result = table_obj.output(["*", "substring(c1, 0, 2)"]).filter("substring(c1, 0, 2) = 'ab'").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "substring(c1, 0, 4)"]).filter("substring(c1, 0, 4) = 'test'").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function upper and lower
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*"]).filter("lower('ABC') = c1").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function ltrim, rtrim, trim
res, extra_result = table_obj.output(["*", "ltrim(c1)"]).filter("ltrim(c1) = 'abc'").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "rtrim(c1)"]).filter("rtrim(c1) = 'abc'").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "trim(c1)"]).filter("trim(c1) = 'abc'").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*"]).filter("trim('   abc   ') = rtrim(ltrim('   abc   '))").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function char_position
res, extra_result = table_obj.output(["*", "char_position(c1, 'bc')"]).filter("char_position(c1, c1) <> 0").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

# math functions
db_obj.drop_table("function_example", ConflictType.Ignore)
db_obj.create_table("function_example",
                    {"c1": {"type": "integer"},
                        "c2": {"type": "double"}}, ConflictType.Error)
table_obj = db_obj.get_table("function_example")
table_obj.insert(
    [{"c1": 1, "c2": 2.4}, {"c1": 3, "c2": 4.5}, {"c1": 5, "c2": 6.6}, {"c1": 7, "c2": 8},
        {"c1": 9, "c2": 10}, {"c1": 11, "c2": 12}, {"c1": 13, "c2": 14}, {"c1": 15, "c2": 16},])

#function sqrt
res, extra_result = table_obj.output(["*", "sqrt(c1)", "sqrt(c2)"]).to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res, extra_result = table_obj.output(["*", "sqrt(c1)", "sqrt(c2)"]).filter("sqrt(c1) = 3").to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function round
res, extra_result = table_obj.output(["*", "round(c1)", "round(c2)"]).to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function ceiling
res, extra_result = table_obj.output(["*", "ceil(c1)", "ceil(c2)"]).to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function floor
res, extra_result = table_obj.output(["*", "floor(c1)", "floor(c2)"]).to_df()
print(res)
if extra_result is not None:
    print(extra_result)

#function ln
res, extra_result = table_obj.output(["*", "ln(c1)", "ln(c2)"]).to_df()
print(res)
if extra_result is not None:
    print(extra_result)

res = db_obj.drop_table("function_example")

hybridsearch_obj.disconnect()