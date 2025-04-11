from hybridsearch_runner import hybridsearchRunner, hybridsearch_runner_decorator_factory
from common import common_values
from hybridsearch.common import ConflictType
from hybridsearch.errors import ErrorCode


def test_0(hybridsearch_runner: hybridsearchRunner):
    hybridsearch_runner.clear()
    uri = common_values.TEST_LOCAL_HOST

    decorator = hybridsearch_runner_decorator_factory(None, uri, hybridsearch_runner)

    @decorator
    def part(hybridsearch_obj):
        db_obj = hybridsearch_obj.create_database("my_db", ConflictType.Error)
        assert db_obj is not None
        res = hybridsearch_obj.drop_database("my_db", ConflictType.Error)
        assert res.error_code == ErrorCode.OK

    for i in range(2):
        print(f"i: {i}")
        part()
