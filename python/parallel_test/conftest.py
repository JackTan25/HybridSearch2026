
import os
from shutil import copyfile

import hybridsearch
import pytest
from hybridsearch.errors import ErrorCode
from hybridsearch.remote_thrift.client import ThrifthybridsearchClient
from hybridsearch.connection_pool import ConnectionPool


from common import common_values
import logging


@pytest.fixture(scope="function")
def connect_hybridsearch():
    return hybridsearch.connect(common_values.TEST_LOCAL_HOST)


@pytest.fixture(scope="function")
def disconnect_hybridsearch():
    res = ThrifthybridsearchClient(common_values.TEST_LOCAL_HOST).disconnect()
    assert res.error_code == ErrorCode.OK


@pytest.fixture(scope="function")
def get_hybridsearch_db():
    # connect
    hybridsearch_obj = hybridsearch.connect(common_values.TEST_LOCAL_HOST)

    yield hybridsearch_obj.get_database("default_db")

    # disconnect
    res = hybridsearch_obj.disconnect()
    assert res.error_code == ErrorCode.OK

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


def pytest_addoption(parser):
    parser.addoption(
        "--run-integration",
        action="store_true",
        default=False,
        help="Run integration tests (requires S3 buckets to be setup with access)",
    )

log_output_file = "run_parallel_test.log"

def pytest_configure(config):
    logger = logging.getLogger("run_parallel_test")
    logger.setLevel(logging.INFO)
    handler = logging.FileHandler(log_output_file, delay=True)
    logger.addHandler(handler)
    logger.addHandler(logging.StreamHandler())
    formatter = logging.Formatter('%(asctime)s - %(threadName)s - %(levelname)s - %(message)s')
    handler.setFormatter(formatter)
