
import os
from shutil import copyfile
import sys

current_path = os.path.abspath(os.path.dirname(__file__))
parent = os.path.join(os.getcwd(), os.pardir)
pparent = os.path.join(parent, os.pardir)
local_hybridsearch_path = os.path.abspath(pparent)
current_python_path = os.path.abspath(pparent) + '/python'

print(current_path, local_hybridsearch_path)

if local_hybridsearch_path in sys.path:
    sys.path.remove(local_hybridsearch_path)

if current_python_path in sys.path:
    sys.path.remove(current_python_path)

print(sys.path)

import hybridsearch
import pytest
from hybridsearch.errors import ErrorCode
from hybridsearch.remote_thrift.client import ThrifthybridsearchClient
from hybridsearch.connection_pool import ConnectionPool


from common import common_values


@pytest.fixture(scope="function")
def connect_hybridsearch():
    return hybridsearch.connect(common_values.TEST_LOCAL_HOST)


@pytest.fixture(scope="function")
def disconnect_hybridsearch():
    res = ThrifthybridsearchClient(common_values.TEST_LOCAL_HOST).disconnect()
    assert res.error_code == ErrorCode.OK


@pytest.fixture(scope="function")
def get_hybridsearch_db_param(request):
    uri = request.param if hasattr(request, 'param') else common_values.TEST_LOCAL_HOST
    # connect
    hybridsearch_obj = hybridsearch.connect(uri)

    yield hybridsearch_obj.get_database("default_db")

    # disconnect
    res = hybridsearch_obj.disconnect()
    assert res.error_code == ErrorCode.OK

@pytest.fixture(scope="function")
def get_hybridsearch_db(request):
    if request.config.getoption("--local-hybridsearch"):
        uri = common_values.TEST_LOCAL_PATH
    else:
        uri = common_values.TEST_LOCAL_HOST

    # connect
    hybridsearch_obj = hybridsearch.connect(uri)
    yield hybridsearch_obj.get_database("default_db")

    # disconnect
    res = hybridsearch_obj.disconnect()
    assert res.error_code == ErrorCode.OK

@pytest.fixture(scope="function")
def skip_if_local_hybridsearch(request):
    if request.config.getoption("--local-hybridsearch"):
        pytest.skip("Skipping local-hybridsearch test")
    
@pytest.fixture(scope="function")
def skip_if_remote_hybridsearch(request):
    if not request.config.getoption("--local-hybridsearch") and not request.config.getoption("--http"):
        pytest.skip("Skipping remote-hybridsearch test")

@pytest.fixture(scope="function")
def skip_if_http(request):
    if request.config.getoption("--http"):
        pytest.skip("Skipping http test")

@pytest.fixture(scope="function", autouse=False)
def get_hybridsearch_connection_pool():
    connection_pool = ConnectionPool(common_values.TEST_LOCAL_HOST)
    yield connection_pool
    connection_pool.destroy()


@pytest.fixture(scope="class")
def check_data(request):
    file_name = request.param["file_name"]
    data_dir = request.param["data_dir"]
    # path not exists
    if not os.path.exists(data_dir):
        os.makedirs(data_dir, exist_ok=True)
        return False
    if not os.path.exists(data_dir + file_name):
        return False
    return True


def disable_items_with_mark(items, mark, reason):
    skipper = pytest.mark.skip(reason=reason)
    for item in items:
        if mark in item.keywords:
            item.add_marker(skipper)

@pytest.fixture
def local_hybridsearch(request):
    return request.config.getoption("--local-hybridsearch")

@pytest.fixture
def http(request):
    return request.config.getoption("--http")

@pytest.fixture
def suffix(request):
    if request.config.getoption("--http"):
        return "_http"
    else :
        return "_remote"

def pytest_addoption(parser):
    parser.addoption(
        "--run-integration",
        action="store_true",
        default=False,
        help="Run integration tests (requires S3 buckets to be setup with access)",
    )

    parser.addoption(
        "--local-hybridsearch",
        action="store_true",
        default=False,
        help="Run local hybridsearch tests (default is remote)",
    )

    parser.addoption(
        "--http",
        action="store_true",
        default=False,
        help="Run http api hybridsearch tests (default is remote)",
    )