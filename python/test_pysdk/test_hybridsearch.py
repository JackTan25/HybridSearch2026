import sys
import os
import pytest
import hybridsearch
import hybridsearch_embedded
from hybridsearch.errors import ErrorCode
from hybridsearch.remote_thrift.client import ThrifthybridsearchClient
from common import common_values
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
from hybridsearch_http import hybridsearch_http


@pytest.mark.usefixtures("local_hybridsearch")
@pytest.mark.usefixtures("http")
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
        self.hybridsearch_obj.disconnect()

    @pytest.mark.usefixtures("skip_if_local_hybridsearch")
    def test_get_hybridsearch(self):
        hybridsearch_obj = ThrifthybridsearchClient(common_values.TEST_LOCAL_HOST)
        database_res = hybridsearch_obj.get_database("default_db")
        print(type(database_res))
        assert database_res.error_code == ErrorCode.OK
        # disconnect
        res = hybridsearch_obj.disconnect()
        assert res.error_code == ErrorCode.OK

    def test_list_hybridsearch(self):
        database_res = self.hybridsearch_obj.list_databases()
        assert "default_db" in database_res.db_names
