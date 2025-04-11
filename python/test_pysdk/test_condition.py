import sys
import os
import pytest
from common import common_values
from sqlglot import condition
from hybridsearch.remote_thrift.table import traverse_conditions
import hybridsearch
import hybridsearch_embedded
from hybridsearch.errors import ErrorCode
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

    def test_condition(self):
        res = (condition("c1 > 1 and c2 < 2 or c3 = 3.3"))
        print(res)
        res = traverse_conditions(condition("c1 = 1"))
        print(res)
        res = traverse_conditions(condition("-8 < c1 and c1 <= -7"))
        print(res)
        res = traverse_conditions(
            condition("(-7 < c1 or 9 <= c1) and (c1 = 3)"))
        print(res)
