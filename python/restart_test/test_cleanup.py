import os
import time
from hybridsearch_runner import hybridsearchRunner, hybridsearch_runner_decorator_factory
import pytest
from common import common_values
from hybridsearch.common import ConflictType
from restart_util import *
import hybridsearch
import pathlib
from hybridsearch.errors import ErrorCode


# Test with cleanuped data but meta data not cleanuped
class TestCleanup:
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
        ],
    )
    def test_cleanuped_data(
        self,
        hybridsearch_runner: hybridsearchRunner,
        columns: dict,
        data_gen_factory,
    ):
        config = "test/data/config/restart_test/test_cleanup/1.toml"
        data_dir = "/var/hybridsearch/data"
        uri = common_values.TEST_LOCAL_HOST
        hybridsearch_runner.clear()
        table_name = "test_cleanup"
        table_name2 = "test2_cleanup"

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            db_obj.drop_table(table_name2, ConflictType.Ignore)
            table_obj = db_obj.create_table(table_name, columns, ConflictType.Error)
            table_obj2 = db_obj.create_table(table_name2, columns, ConflictType.Error)

            insert_n = 100
            data_gen = data_gen_factory(insert_n)

            for data in data_gen:
                data_line = {}
                for column_name, column_data in zip(columns.keys(), data):
                    data_line[column_name] = column_data
                table_obj.insert([data_line])
                table_obj2.insert([data_line])

            db_obj.drop_table(table_name, ConflictType.Error)

            hybridsearch_obj.cleanup()

            # check
            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{table_name}*")
            assert len(list(dropped_dirs)) == 0

            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{table_name2}*")
            assert len(list(dropped_dirs)) == 1

            db_obj.drop_table(table_name2, ConflictType.Error)

        part1()

        @decorator
        def part2(hybridsearch_obj):
            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{table_name2}*")
            assert len(list(dropped_dirs)) == 0

        part2()

    @pytest.mark.parametrize(
        "columns, indexes, data_gen_factory",
        [
            (
                SimpleEmbeddingGenerator.columns(),
                SimpleEmbeddingGenerator.index(),
                SimpleEmbeddingGenerator.gen_factory(),
            ),
            (
                SimpleVarcharGenerator.columns(),
                SimpleVarcharGenerator.index(),
                SimpleVarcharGenerator.gen_factory(),
            ),
        ],
    )
    def test_cleanuped_index(
        self,
        hybridsearch_runner: hybridsearchRunner,
        columns: dict,
        indexes: list[index.IndexInfo],
        data_gen_factory,
    ):
        config = "test/data/config/restart_test/test_cleanup/1.toml"
        data_dir = "/var/hybridsearch/data"
        uri = common_values.TEST_LOCAL_HOST
        hybridsearch_runner.clear()
        table_name = "test_cleanup_index"
        index_name = "idx1"
        index_name2 = "idx2"

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)
        insert_n = 100

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            table_obj = db_obj.create_table(table_name, columns, ConflictType.Error)

            for idx in indexes:
                res = table_obj.create_index(index_name, idx)
                assert res.error_code == hybridsearch.ErrorCode.OK

                res = table_obj.create_index(index_name2, idx)
                assert res.error_code == hybridsearch.ErrorCode.OK

            data_gen = data_gen_factory(insert_n)

            for data in data_gen:
                data_line = {}
                for column_name, column_data in zip(columns.keys(), data):
                    data_line[column_name] = column_data
                table_obj.insert([data_line])

            res = table_obj.drop_index(index_name)
            assert res.error_code == hybridsearch.ErrorCode.OK

            hybridsearch_obj.cleanup()

            # check
            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{index_name}*")
            assert len(list(dropped_dirs)) == 0

            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{index_name2}*")
            assert len(list(dropped_dirs)) == 1

            res = table_obj.drop_index(index_name2)

        part1()

        @decorator
        def part2(hybridsearch_obj):
            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{index_name2}")
            assert len(list(dropped_dirs)) == 0

            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.get_table(table_name)
            data_dict, _, _ = table_obj.output(["count(*)"]).to_result()
            count_star = data_dict["count(star)"][0]
            assert count_star == insert_n

            db_obj.drop_table(table_name, ConflictType.Error)

            hybridsearch_obj.cleanup()

            dropped_dirs = pathlib.Path(data_dir).rglob(f"*{table_name}*")
            assert len(list(dropped_dirs)) == 0

        part2()

    def test_invalidate_fulltext_cache(self, hybridsearch_runner: hybridsearchRunner):
        table_name = "test_invalid_fulltext_cache"
        config = "test/data/config/restart_test/test_cleanup/1.toml"
        uri = common_values.TEST_LOCAL_HOST
        hybridsearch_runner.clear()

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            table_obj = db_obj.create_table(
                table_name,
                {
                    "c1": {"type": "varchar"},
                    "c2": {"type": "varchar"},
                },
            )
            table_obj.insert([{"c1": "text1", "c2": "text2"}])

            drop_index_name = "idx1_todrop"
            table_obj.create_index(
                drop_index_name, index.IndexInfo("c1", index.IndexType.FullText)
            )
            table_obj.create_index(
                "idx2", index.IndexInfo("c2", index.IndexType.FullText)
            )

            res = (
                table_obj.output(["c1"])
                .match_text(fields="c1", matching_text="text1", topn=1)
                .to_result()
            )

            table_obj.drop_index(drop_index_name)

            hybridsearch_obj.cleanup()
            dropped_index_dirs = pathlib.Path("/var/hybridsearch/data").rglob(
                f"*{drop_index_name}*"
            )
            assert len(list(dropped_index_dirs)) == 0

            db_obj.drop_table(table_name)

        part1()

    def test_invalidate_fulltext_cache2(self, hybridsearch_runner: hybridsearchRunner):
        table_name = "test_invalid_fulltext_cache2"
        index_name = "body_index"

        data_dir = "/var/hybridsearch/data"
        config = "test/data/config/restart_test/test_cleanup/2.toml"
        uri = common_values.TEST_LOCAL_HOST
        import_file = "test/data/csv/enwiki_9.csv"
        abs_import_file = os.path.abspath(import_file)

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            table_obj = db_obj.create_table(
                table_name,
                {
                    "doctitle": {"type": "varchar"},
                    "docdate": {"type": "varchar"},
                    "body": {"type": "varchar"},
                },
                ConflictType.Error,
            )
            res = table_obj.create_index(
                index_name, index.IndexInfo("body", index.IndexType.FullText)
            )
            assert res.error_code == ErrorCode.OK

            # compaction need 4 segment in default config
            for i in range(4):
                table_obj.import_data(abs_import_file, {"delimiter": "\t"})

            res = table_obj.output(["*"]).match_text("body^5", "America", 3).to_pl()

            # wait for compaction
            time.sleep(3)

            hybridsearch_obj.cleanup()
            time.sleep(1)

            cnt = 0
            for path in pathlib.Path(data_dir).rglob("*"):
                depth = len(path.relative_to(data_dir).parts)
                # print("    " * depth + path.name)
                if path.is_dir() and index_name in str(path):
                    cnt += 1
            assert cnt == 1

            db_obj.drop_table(table_name)

        part1()
