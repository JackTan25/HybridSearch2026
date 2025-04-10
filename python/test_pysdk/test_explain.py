import importlib
import sys
import os
from common import common_values
import hybridsearch
import hybridsearch_embedded
from hybridsearch.errors import ErrorCode
from hybridsearch.table import ExplainType
from hybridsearch.common import ConflictType
import polars as pl
import pytest
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
from hybridsearch_http import hybridsearch_http

@pytest.mark.usefixtures("local_hybridsearch")
@pytest.mark.usefixtures("http")
@pytest.mark.usefixtures("suffix")
class Testhybridsearch:
    @pytest.fixture(autouse=True)
    def setup(self, local_hybridsearch, http):
        if local_hybridsearch:
            module = importlib.import_module("hybridsearch_embedded.common")
            func = getattr(module, 'ConflictType')
            globals()['ConflictType'] = func
            module = importlib.import_module("hybridsearch_embedded.table")
            func = getattr(module, 'ExplainType')
            globals()['ExplainType'] = func
            self.uri = common_values.TEST_LOCAL_PATH
            self.hybridsearch_obj = hybridsearch_embedded.connect(self.uri)
        elif http:
           self.uri = common_values.TEST_LOCAL_HOST
           self.hybridsearch_obj = hybridsearch_http()
        else:
            self.uri = common_values.TEST_LOCAL_HOST
            self.hybridsearch_obj = hybridsearch.connect(self.uri)
        assert self.hybridsearch_obj

    def teardown(self):
        res = self.hybridsearch_obj.disconnect()
        assert res.error_code == ErrorCode.OK

    def test_explain(self, suffix):
        """
            # Analyze = 1
            # Ast = 2
            # UnOpt = 3
            # Opt = 4
            # Physical = 5
            # Pipeline = 6
            # Fragment = 7
        """
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table("test_explain_default"+suffix, ConflictType.Ignore)
        table = db_obj.create_table("test_explain_default"+suffix, {
            "c1": {"type": "varchar", "constraints": ["primary key"]}, "c2": {"type": "float"}}, ConflictType.Error)
        assert table

        table.insert({"c1": "hello", "c2": 1.0})
        table.insert({"c1": "world", "c2": 2.0})
        table.insert({"c1": "hello", "c2": 3.0})

        with pl.Config(fmt_str_lengths=1000):
            res = table.output(["*"]).explain()
            print(res)

            res = table.output(["*"]).explain(ExplainType.Ast)
            print(res)

            res = table.output(["*"]).explain(ExplainType.UnOpt)
            print(res)

            res = table.output(["*"]).explain(ExplainType.Opt)
            print(res)

            res = table.output(["*"]).explain(ExplainType.Physical)
            print(res)

            res = table.output(["*"]).explain(ExplainType.Pipeline)
            print(res)

            res = table.output(["*"]).explain(ExplainType.Fragment)
            print(res)

            with pytest.raises(Exception, match=r".*Not implement*"):
                res = table.output(["*"]).explain(ExplainType.Analyze)
                print(res)

        db_obj.drop_table("test_explain_default"+suffix, ConflictType.Error)