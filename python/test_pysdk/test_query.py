import importlib
import sys
import os
import pytest
from common import common_values
import hybridsearch
import hybridsearch_embedded
import hybridsearch.index as index
from hybridsearch.errors import ErrorCode
from hybridsearch.remote_thrift.client import ThrifthybridsearchClient
from hybridsearch.remote_thrift.db import RemoteDatabase
from hybridsearch.remote_thrift.query_builder import hybridsearchThriftQueryBuilder
from hybridsearch.remote_thrift.table import RemoteTable
from hybridsearch.common import ConflictType
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
from hybridsearch_http import hybridsearch_http


@pytest.fixture(scope="class")
def local_hybridsearch(request):
    return request.config.getoption("--local-hybridsearch")


@pytest.fixture(scope="class")
def http(request):
    return request.config.getoption("--http")


@pytest.fixture(scope="class")
def setup_class(request, local_hybridsearch, http):
    if local_hybridsearch:
        module = importlib.import_module("hybridsearch_embedded.index")
        globals()["index"] = module
        uri = common_values.TEST_LOCAL_PATH
        request.cls.hybridsearch_obj = hybridsearch_embedded.connect(uri)
    elif http:
        uri = common_values.TEST_LOCAL_HOST
        request.cls.hybridsearch_obj = hybridsearch_http()
    else:
        uri = common_values.TEST_LOCAL_HOST
        request.cls.hybridsearch_obj = hybridsearch.connect(uri)
    request.cls.uri = uri
    yield
    request.cls.hybridsearch_obj.disconnect()


@pytest.mark.usefixtures("setup_class")
class Testhybridsearch:
    @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    @pytest.mark.usefixtures("skip_if_http")
    def test_query(self):
        conn = ThrifthybridsearchClient(common_values.TEST_LOCAL_HOST)
        db = RemoteDatabase(conn, "default_db")
        db.drop_table("my_table", conflict_type=ConflictType.Ignore)
        db.create_table(
            "my_table", {
                "num": {"type": "integer"}, "body": {"type": "varchar"}, "vec": {"type": "vector,5,float"}},
            ConflictType.Error)

        table = RemoteTable(conn, "default_db", "my_table")
        res = table.insert(
            [{"num": 1, "body": "undesirable, unnecessary, and harmful", "vec": [1.0] * 5}])
        assert res.error_code == ErrorCode.OK
        res = table.insert(
            [{"num": 2, "body": "publisher=US National Office for Harmful Algal Blooms", "vec": [4.0] * 5}])
        assert res.error_code == ErrorCode.OK
        res = table.insert(
            [{"num": 3, "body": "in the case of plants, growth and chemical", "vec": [7.0] * 5}])
        assert res.error_code == ErrorCode.OK

        res = table.create_index("my_index",
                                 index.IndexInfo("body",
                                                 index.IndexType.FullText),
                                 ConflictType.Error)
        assert res.error_code == ErrorCode.OK

        # select_res = table.query_builder().output(["*"]).to_df()
        # print(select_res)
        # Create a query builder
        query_builder = hybridsearchThriftQueryBuilder(table)
        query_builder.output(["num", "body"])
        query_builder.match_dense('vec', [3.0] * 5, 'float', 'ip', 2)
        query_builder.match_text('body', 'harmful', 2, None)
        query_builder.fusion(method='rrf', topn=10, fusion_params=None)
        res, extra_result = query_builder.to_df()
        print(res)
        res = table.drop_index("my_index", ConflictType.Error)
        assert res.error_code == ErrorCode.OK

        res = db.drop_table("my_table", ConflictType.Error)
        assert res.error_code == ErrorCode.OK

        res = conn.disconnect()
        assert res.error_code == ErrorCode.OK

    @pytest.mark.usefixtures("skip_if_http")
    def test_query_builder(self):
        # connect
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table("test_query_builder",
                          conflict_type=ConflictType.Ignore)
        table_obj = db_obj.create_table(
            "test_query_builder", {"c1": {"type": "int"}}, ConflictType.Error)
        query_builder = table_obj.query_builder
        query_builder.output(["*"]).to_df()

        res = db_obj.drop_table("test_query_builder", ConflictType.Error)
        assert res.error_code == ErrorCode.OK
