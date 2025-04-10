import hybridsearch
from common import common_values
from hybridsearch_runner import hybridsearchRunner, hybridsearch_runner_decorator_factory
import time
from hybridsearch.errors import ErrorCode


class TestDrop:
    # drop table will drop all wal before so delete will not be replayed
    def test_drop_and_replay(self, hybridsearch_runner: hybridsearchRunner):
        config = "test/data/config/restart_test/test_drop/1.toml"
        uri = common_values.TEST_LOCAL_HOST
        hybridsearch_runner.clear()

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):

            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.create_table("test_drop", {"c1": {"type": "int"}})

            res = table_obj.insert([{"c1": i} for i in range(10)])
            assert res.error_code == ErrorCode.OK
            # wait flush
            time.sleep(2)

            table_obj.delete("c1 < 5")

            db_obj.drop_table("test_drop")

            # wait for the drop be flushed in delta ckp
            time.sleep(3)

        part1()

        # replay nothing
        @decorator
        def part2(hybridsearch_obj):

            db_obj = hybridsearch_obj.get_database("default_db")
            try:
                table_obj = db_obj.get_table("test_drop")
            except Exception as e:
                assert e.error_code == ErrorCode.TABLE_NOT_EXIST

            db_obj.drop_table("test_drop")

        part2()
