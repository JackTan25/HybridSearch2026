import random
import pytest
from common import common_values
from hybridsearch.common import ConflictType
from hybridsearch_runner import hybridsearchRunner, hybridsearch_runner_decorator_factory
import time
import threading
from restart_util import *
from util import RtnThread


class TestInsert:
    def insert_inner(
        self,
        insert_n: int,
        hybridsearch_runner: hybridsearchRunner,
        config: str,
        columns: dict,
        data_gen_factory,
    ):
        uri = common_values.TEST_LOCAL_HOST
        data_gen = data_gen_factory(insert_n)

        stop_n = 10
        uri = common_values.TEST_LOCAL_HOST

        cur_insert_n = 0
        shutdown = False
        error = False

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner, shutdown_out=True)

        def insert_func(table_obj):
            nonlocal cur_insert_n, shutdown, error
            batch_size = 10

            while cur_insert_n < insert_n:
                insert_data = []
                try:
                    # get `batch_size` data in data_gen one time
                    for i in range(batch_size):
                        try:
                            data = next(data_gen)
                            data_line = {}
                            for col_name, col_data in zip(columns.keys(), data):
                                data_line[col_name] = col_data
                            insert_data.append(data_line)
                        except StopIteration:
                            break
                    if len(insert_data) > 0:
                        table_obj.insert(insert_data)
                    else:
                        cur_insert_n = insert_n
                except Exception as e:
                    print(f"insert error at {cur_insert_n}")
                    if not shutdown:
                        error = True
                        raise e
                    break
                cur_insert_n += len(insert_data)

        shutdown_time = 0

        def shutdown_func():
            nonlocal cur_insert_n, shutdown_time, shutdown, error
            shutdown = False
            last_shutdown_insert_n = cur_insert_n
            while not error:
                if cur_insert_n >= insert_n or (
                    stop_n != 0
                    and cur_insert_n - last_shutdown_insert_n >= insert_n // stop_n
                ):
                    shutdown = True
                    hybridsearch_runner.uninit()
                    print("shutdown hybridsearch")
                    shutdown_time += 1
                    return
                print(f"cur_insert_n inner: {cur_insert_n}")
                time.sleep(0.1)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.get_table("test_insert")

            data_dict, _, _ = table_obj.output(["count(*)"]).to_result()
            count_star = data_dict["count(star)"][0]
            assert count_star == cur_insert_n
            print(f"cur_insert_n: {cur_insert_n}")

            t1 = threading.Thread(target=shutdown_func)
            t1.start()
            insert_func(table_obj)
            t1.join()

        while cur_insert_n < insert_n:
            part1()

    @pytest.mark.slow
    @pytest.mark.parametrize(
        "insert_n, config",
        [
            (100000, "test/data/config/restart_test/test_insert/1.toml"),
            # (1000000, "test/data/config/restart_test/test_insert/1.toml"),
        ],
    )
    @pytest.mark.parametrize(
        "columns, data_gen_factory",
        [
            (
                SimpleEmbeddingGenerator.columns(),
                SimpleEmbeddingGenerator.gen_factory(),
            ),
            (
                SimpleVarcharGenerator.columns(),
                SimpleVarcharGenerator.gen_factory(),
            ),
            (
                SimpleTensorGenerator.columns(),
                SimpleTensorGenerator.gen_factory(),
            ),
            (
                EnwikiGenerator.columns(),
                EnwikiGenerator.gen_factory("test/data/benchmark/enwiki-10w.csv"),
            ),
            (
                SiftGenerator.columns(),
                SiftGenerator.gen_factory(
                    "test/data/benchmark/sift_1m/sift_base.fvecs"
                ),
            ),
            (
                LChYDataGenerato.columns(),
                LChYDataGenerato.gen_factory("test/data/jsonl/test_table.jsonl"),
            ),
        ],
    )
    def test_data(
        self,
        hybridsearch_runner: hybridsearchRunner,
        insert_n: int,
        config: str,
        columns: dict,
        data_gen_factory,
    ):
        uri = common_values.TEST_LOCAL_HOST
        hybridsearch_runner.clear()

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.create_table("test_insert", columns, ConflictType.Error)
        part1()

        self.insert_inner(insert_n, hybridsearch_runner, config, columns, data_gen_factory)

        @decorator
        def part2(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table("test_insert", ConflictType.Error)
        part2()

    @pytest.mark.slow
    @pytest.mark.parametrize(
        "insert_n,config",
        [
            (100000, "test/data/config/restart_test/test_insert/1.toml"),
            # (1000000, "test/data/config/restart_test/test_insert/1.toml"),
        ],
    )
    @pytest.mark.parametrize(
        "columns, indexes, data_gen_factory",
        [
            (
                EnwikiGenerator.columns(),
                EnwikiGenerator.index(),
                EnwikiGenerator.gen_factory("test/data/benchmark/enwiki-10w.csv"),
            ),
            (
                SiftGenerator.columns(),
                SiftGenerator.index(),
                SiftGenerator.gen_factory(
                    "test/data/benchmark/sift_1m/sift_base.fvecs"
                ),
            ),
            (
                LChYDataGenerato.columns(),
                LChYDataGenerato.index(),
                LChYDataGenerato.gen_factory("test/data/jsonl/test_table.jsonl"),
            ),
        ],
    )
    def test_index(
        self,
        hybridsearch_runner: hybridsearchRunner,
        insert_n: int,
        config: str,
        columns: dict,
        indexes: list[index.IndexInfo],
        data_gen_factory,
    ):
        uri = common_values.TEST_LOCAL_HOST
        hybridsearch_runner.clear()

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.create_table("test_insert", columns, ConflictType.Error)
            for idx in indexes:
                table_obj.create_index(f"idx_{idx.column_name}", idx)
        part1()

        self.insert_inner(insert_n, hybridsearch_runner, config, columns, data_gen_factory)

        @decorator
        def part2(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table("test_insert", ConflictType.Error)
        part2()

    def _test_insert_checkpoint_inner(
        self, hybridsearch_runner: hybridsearchRunner, test_n: int
    ):
        hybridsearch_runner.clear()
        config = "test/data/config/restart_test/test_insert/6.toml"
        uri = common_values.TEST_LOCAL_HOST

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        line_num = 0

        @decorator
        def create_table(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.create_table("test_insert_checkpoint", {"c1": {"type": "int"}})

        @decorator
        def part1(hybridsearch_obj, test_i: int):
            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.get_table("test_insert_checkpoint")

            data_dict, _, _ = table_obj.output(["count(*)"]).to_result()
            count_star = data_dict["count(star)"][0]
            assert count_star == line_num

            stop_insert = False

            def checkpoint_func(hybridsearch_obj):
                nonlocal stop_insert

                try:
                    time.sleep(0.2)
                    hybridsearch_obj.flush_data()
                    time.sleep(0.2)
                    hybridsearch_obj.flush_delta()
                    time.sleep(0.2)
                except Exception as e:
                    print(e)
                    stop_insert = True
                    raise e
                stop_insert = True

            def insert_func(table_obj):
                nonlocal stop_insert, line_num
                while not stop_insert:
                    table_obj.insert([{"c1": i} for i in range(8192)])
                    line_num += 8192
                print("insert end")

            t1 = RtnThread(target=insert_func, args=(table_obj,))
            t1.start()
            checkpoint_func(hybridsearch_obj)
            t1.join()
            print(f"join end {test_i}")

        @decorator
        def drop_table(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table("test_insert_checkpoint", ConflictType.Error)

        create_table()
        for i in range(test_n):
            part1(i)
        drop_table()

    def test_insert_checkpoint(self, hybridsearch_runner: hybridsearchRunner):
        self._test_insert_checkpoint_inner(hybridsearch_runner, 10)

    @pytest.mark.slow
    def test_insert_checkpoint_slow(self, hybridsearch_runner: hybridsearchRunner):
        self._test_insert_checkpoint_inner(hybridsearch_runner, 100)


if __name__ == "__main__":
    pass
