from functools import wraps
import pathlib
import time
import pytest
from hybridsearch_runner import hybridsearchRunner, hybridsearch_runner_decorator_factory
from common import common_values
from hybridsearch.common import ConflictType
from hybridsearch.errors import ErrorCode
import pandas as pd
from numpy import dtype
import hybridsearch.index as index


class TestAlter:
    def test_alter_simple(self, hybridsearch_runner: hybridsearchRunner):
        table_name = "test_alter1"
        config = "test/data/config/restart_test/test_alter/1.toml"

        hybridsearch_runner.clear()
        uri = common_values.TEST_LOCAL_HOST

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            table_obj = db_obj.create_table(
                table_name,
                {
                    "c1": {"type": "int"},
                    "c2": {"type": "int"},
                    "c3": {"type": "varchar"},
                },
            )
            res = table_obj.create_index(
                "index1", index.IndexInfo("c3", index.IndexType.FullText)
            )
            assert res.error_code == ErrorCode.OK

            table_obj.insert([{"c1": 1, "c2": 2, "c3": "test"}])

            res = table_obj.add_columns(
                {"c4": {"type": "varchar", "default": "default"}}
            )
            assert res.error_code == ErrorCode.OK

            res = table_obj.drop_columns(["c2"])
            assert res.error_code == ErrorCode.OK

            table_obj.insert([{"c1": 2, "c3": "test", "c4": "test2"}])

        @decorator
        def part2(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.get_table(table_name)

            res, extra_result = table_obj.output(["*"]).to_df()
            pd.testing.assert_frame_equal(
                res,
                pd.DataFrame(
                    {"c1": [1, 2], "c3": ["test", "test"], "c4": ["default", "test2"]}
                ).astype(
                    {
                        "c1": dtype("int32"),
                        "c3": dtype("object"),
                        "c4": dtype("object"),
                    }
                ),
            )

            db_obj.drop_table(table_name)

        part1()
        part2()

    @pytest.mark.parametrize(
        "flush, flush_mid",
        [
            (False, False),
            (True, False),
            (True, True),
        ],
    )
    def test_alter_complex(
        self, hybridsearch_runner: hybridsearchRunner, flush: bool, flush_mid: bool
    ):
        config = "test/data/config/restart_test/test_alter/1.toml"
        table_name = "test_alter2"

        hybridsearch_runner.clear()
        uri = common_values.TEST_LOCAL_HOST

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            table_obj = db_obj.create_table(
                table_name,
                {
                    "c1": {"type": "int"},
                    "c2": {"type": "int"},
                    "c3": {"type": "varchar"},
                },
            )
            res = table_obj.create_index(
                "index1", index.IndexInfo("c3", index.IndexType.FullText)
            )
            assert res.error_code == ErrorCode.OK

            table_obj.insert([{"c1": 1, "c2": 2, "c3": "test"}])

            if flush and flush_mid:
                hybridsearch_obj.flush_delta()

            res = table_obj.add_columns({"c4": {"type": "varchar", "default": "tttt"}})
            assert res.error_code == ErrorCode.OK

            res = table_obj.drop_columns(["c2"])
            assert res.error_code == ErrorCode.OK

            res = table_obj.add_columns({"c2": {"type": "float", "default": 0.0}})
            assert res.error_code == ErrorCode.OK

            table_obj.insert([{"c1": 2, "c3": "test2", "c4": "tttt2", "c2": 1.0}])

            res = table_obj.add_columns({"c5": {"type": "int", "default": 0}})
            assert res.error_code == ErrorCode.OK

            if flush:
                hybridsearch_obj.flush_delta()

        @decorator
        def part2(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            table_obj = db_obj.get_table(table_name)
            res, extra_result = table_obj.output(["*"]).to_df()
            pd.testing.assert_frame_equal(
                res,
                pd.DataFrame(
                    {
                        "c1": [1, 2],
                        "c3": ["test", "test2"],
                        "c4": ["tttt", "tttt2"],
                        "c2": [0.0, 1.0],
                        "c5": [0, 0],
                    }
                ).astype(
                    {
                        "c1": dtype("int32"),
                        "c3": dtype("object"),
                        "c4": dtype("object"),
                        "c2": dtype("float32"),
                        "c5": dtype("int32"),
                    }
                ),
            )
            dropped_column_dirs = pathlib.Path("/var/hybridsearch/data").rglob("1.col")
            assert len(list(dropped_column_dirs)) == 0

            res = table_obj.list_indexes()
            assert len(res.index_names) == 1

            data_dict, _, _ = (
                table_obj.output(["c1"])
                .match_text(fields="c3", matching_text="test", topn=1)
                .to_result()
            )
            assert data_dict == {"c1": [1]}

            db_obj.drop_table(table_name)

        part1()
        part2()

    def test_alter_cleanup_simple(self, hybridsearch_runner: hybridsearchRunner):
        table_name = "test_alter3"
        config = "test/data/config/restart_test/test_alter/3.toml"

        hybridsearch_runner.clear()
        uri = common_values.TEST_LOCAL_HOST

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        data_dir = "/var/hybridsearch/data"

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)

            table_obj = db_obj.create_table(
                table_name,
                {
                    "c1": {"type": "int"},
                    "c2": {"type": "int"},
                    "c3": {"type": "varchar"},
                },
            )
            table_obj.insert([{"c1": 1, "c2": 2, "c3": "test"}])

            res = table_obj.drop_columns(["c2"])
            assert res.error_code == ErrorCode.OK

            hybridsearch_obj.cleanup()

            dropped_column_dirs = pathlib.Path(data_dir).rglob("1.col")
            # find the column file with the column idx = 1
            assert len(list(dropped_column_dirs)) == 0

            # contain "0.col" or "2.col"
            column_dirs = pathlib.Path(data_dir).rglob("[02].col")
            assert len(list(column_dirs)) == 2

            res = table_obj.drop_columns(["c3"])
            assert res.error_code == ErrorCode.OK

        part1()

        @decorator
        def part2(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")

            dropped_column_dirs = pathlib.Path(data_dir).rglob("2.col")
            assert len(list(dropped_column_dirs)) == 0

            db_obj.drop_table(table_name)

        part2()

    def test_restart_after_alter_and_checkpoint(self, hybridsearch_runner: hybridsearchRunner):
        table_name = "test_alter4"
        config = "test/data/config/restart_test/test_alter/1.toml"

        hybridsearch_runner.clear()
        uri = common_values.TEST_LOCAL_HOST
        data_dir = "/var/hybridsearch/data"

        decorator = hybridsearch_runner_decorator_factory(config, uri, hybridsearch_runner)

        @decorator
        def part1(hybridsearch_obj):
            db_obj = hybridsearch_obj.get_database("default_db")
            db_obj.drop_table(table_name, ConflictType.Ignore)
            table_obj = db_obj.create_table(
                table_name,
                {
                    "c1": {"type": "int"},
                    "c2": {"type": "int"},
                    "c3": {"type": "varchar"},
                },
            )
            table_obj.insert([{"c1": 1, "c2": 2, "c3": "test"}])

            table_obj.add_columns({"c4": {"type": "varchar", "default": "tttt"}})
            table_obj.drop_columns(["c2"])

            hybridsearch_obj.flush_data()

            table_obj.drop_columns(["c3"])

            hybridsearch_obj.flush_delta()
        
        part1()

        @decorator
        def part2(hybridsearch_obj):
            dropped_column_dirs = pathlib.Path(data_dir).rglob("1.col")
            assert len(list(dropped_column_dirs)) == 0

            dropped_column_dirs = pathlib.Path(data_dir).rglob("2.col")
            assert len(list(dropped_column_dirs)) == 0

        part2()
