import importlib
import sys
import os
import pytest
import polars as pl
from common import common_values
from hybridsearch.common import ConflictType
from hybridsearch.errors import ErrorCode
import hybridsearch
import hybridsearch_embedded

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
        module = importlib.import_module("hybridsearch_embedded.common")
        func = getattr(module, 'ConflictType')
        globals()['ConflictType'] = func
        uri = common_values.TEST_LOCAL_PATH
        request.cls.hybridsearch_obj = hybridsearch_embedded.connect(uri)
    elif http:
        uri = common_values.TEST_LOCAL_HOST
        request.cls.hybridsearch_obj = hybridsearch_http()
    else:
        uri = common_values.TEST_LOCAL_HOST
        request.cls.hybridsearch_obj = hybridsearch_http()
    request.cls.uri = uri
    yield
    request.cls.hybridsearch_obj.disconnect()


@pytest.mark.usefixtures("setup_class")
@pytest.mark.usefixtures("suffix")
class Testhybridsearch:
    def _test_show_table(self, suffix):
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table("test_show_table" + suffix, ConflictType.Ignore)
        db_obj.create_table(
            "test_show_table" + suffix,
            {"num": {"type": "integer"}, "body": {"type": "varchar"}, "vec": {"type": "vector,5,float"}},
            ConflictType.Error)
        with pl.Config(fmt_str_lengths=1000):
            res = db_obj.show_table("test_show_table" + suffix)
            print(res)

        res = db_obj.drop_table("test_show_table" + suffix, ConflictType.Error)
        assert res.error_code == ErrorCode.OK

    # @pytest.mark.usefixtures("skip_if_http")
    # @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    def _test_show_columns(self, suffix):
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table("test_show_columns" + suffix, ConflictType.Ignore)
        table_obj = db_obj.create_table(
            "test_show_columns" + suffix,
            {"num": {"type": "integer"}, "body": {"type": "varchar"}, "vec": {"type": "vector,5,float"}},
            ConflictType.Error)
        with pl.Config(fmt_str_lengths=1000):
            res = table_obj.show_columns()
            print(res)
            # check the polars dataframe
            assert res.columns == ["name", "type", "default", "comment"]

        res = db_obj.drop_table("test_show_columns" + suffix, ConflictType.Error)
        assert res.error_code == ErrorCode.OK

    # @pytest.mark.usefixtures("skip_if_http")
    # @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    def test_show_columns_with_comment(self, suffix):
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table("test_show_columns" + suffix, ConflictType.Ignore)
        table_obj = db_obj.create_table(
            "test_show_columns" + suffix,
            {
                "num": {"type": "integer", "default": 0},
                "body": {
                    "type": "varchar",
                    "default": "default text",
                    "comment": "comment2",
                },
                "vec": {"type": "vector,5,float", "comment": "comment3"},
            },
            ConflictType.Error,
        )
        with pl.Config(fmt_str_lengths=1000):
            res: pl.DataFrame = table_obj.show_columns()
            print(res)
            # check the polars dataframe
            expected_data = [
                {"name": "num", "type": "Integer", "default": "0", "comment": ""},
                {
                    "name": "body",
                    "type": "Varchar",
                    "default": "default text",
                    "comment": "comment2",
                },
                {
                    "name": "vec",
                    "type": "Embedding(float,5)",
                    "default": "Null",
                    "comment": "comment3",
                },
            ]
            expected_df = pl.DataFrame(expected_data)
            print(expected_df)
            assert res.equals(expected_df)

        res = db_obj.drop_table("test_show_columns" + suffix, ConflictType.Error)
        assert res.error_code == ErrorCode.OK

    def _test_show_big_databases(self, suffix):
        for i in range(8193):
            self.hybridsearch_obj.drop_database(f"test_show_big_databases_{i}" + suffix, ConflictType.Ignore)

        for i in range(8193):
            self.hybridsearch_obj.create_database(f"test_show_big_databases_{i}" + suffix, ConflictType.Ignore)

        res = self.hybridsearch_obj.list_databases()
        assert res.error_code == ErrorCode.OK

        for i in range(8193):
            self.hybridsearch_obj.drop_database(f"test_show_big_databases_{i}" + suffix, ConflictType.Ignore)

    def _test_show_current_node(self, suffix):
        res = self.hybridsearch_obj.show_current_node()
        assert res.error_code == ErrorCode.OK
        print(res)

    def test_show(self, suffix):
        self._test_show_table(suffix)
        self._test_show_columns(suffix)
        self._test_show_big_databases(suffix)
        self._test_show_current_node(suffix)

    @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    @pytest.mark.usefixtures("skip_if_remote_hybridsearch")
    # @pytest.mark.skip(reason="Cannot show follower number")
    def test_show_global_variables(self, suffix):
        vars = self.hybridsearch_obj.show_global_variables()
        print(vars)

    @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    @pytest.mark.usefixtures("skip_if_remote_hybridsearch")
    def test_show_global_variable(self, suffix):
        var: dict = self.hybridsearch_obj.show_global_variable("cache_result_capacity")
        assert var["error_code"] == ErrorCode.OK
        assert "cache_result_capacity" in var

        var = self.hybridsearch_obj.show_global_variable("cache_result_num")
        assert var["error_code"] == ErrorCode.OK
        assert "cache_result_num" in var

        var = self.hybridsearch_obj.show_global_variable("result_cache")
        assert var["error_code"] == ErrorCode.OK
        assert "result_cache" in var

        try:
            var = self.hybridsearch_obj.show_global_variable("invalid_variable")
        except Exception as e:
            assert e.error_code == ErrorCode.NO_SUCH_SYSTEM_VAR
        else:
            raise Exception("Should raise exception")

    @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    @pytest.mark.usefixtures("skip_if_remote_hybridsearch")
    def test_set_config(self, suffix):
        res = self.hybridsearch_obj.set_config({"cache_result_capacity": 100})
        assert res.error_code == ErrorCode.OK

        res = self.hybridsearch_obj.set_config({"result_cache": "clear"})
        assert res.error_code == ErrorCode.OK

        try:
            res = self.hybridsearch_obj.set_config({"invalid_variable": "value"})
        except Exception as e:
            assert e.error_code == ErrorCode.INVALID_COMMAND
        else:
            raise Exception("Should raise exception")
