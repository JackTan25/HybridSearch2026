import pytest
from hybridsearch_cluster import hybridsearchCluster
from mocked_hybridsearch_cluster import MockhybridsearchCluster


def pytest_addoption(parser):
    parser.addoption(
        "--hybridsearch_path",
        action="store",
        default="cmake-build-debug/src/hybridsearch",
    )
    parser.addoption(
        "--use_sudo",
        action="store_true",
        default=False,
        help="Use sudo to run command",
    )


def pytest_generate_tests(metafunc):
    hybridsearch_path = metafunc.config.getoption("hybridsearch_path")

    use_sudo = metafunc.config.getoption("use_sudo")

    test_name = metafunc.function.__name__

    if "cluster" in metafunc.fixturenames:
        hybridsearch_cluster = hybridsearchCluster(hybridsearch_path, test_name=test_name)
        metafunc.parametrize("cluster", [hybridsearch_cluster])
    elif "mock_cluster" in metafunc.fixturenames:
        mock_hybridsearch_cluster = MockhybridsearchCluster(
            hybridsearch_path,
            test_name=test_name,
            use_sudo=use_sudo,
        )
        metafunc.parametrize("mock_cluster", [mock_hybridsearch_cluster])
