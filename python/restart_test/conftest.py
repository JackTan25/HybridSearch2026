from hybridsearch_runner import hybridsearchRunner
from restart_timeout import *


def pytest_addoption(parser):
    parser.addoption(
        "--hybridsearch_path",
        action="store",
        default="./build/Debug/src/hybridsearch",
    )
    parser.addoption(
        "--builder_container",
        action="store",
    )


def pytest_generate_tests(metafunc):
    if "hybridsearch_runner" in metafunc.fixturenames:
        hybridsearch_path = metafunc.config.getoption("hybridsearch_path")
        runner = hybridsearchRunner(hybridsearch_path)
        metafunc.parametrize("hybridsearch_runner", [runner])


# def pytest_collection_modifyitems(config, items):
#     for item in items:
#         # Apply the decorator to each test function
#         test_name = item.name
#         item.obj = my_timeout(test_name)(item.obj)
