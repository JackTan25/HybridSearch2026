from numpy import dtype
import pytest
import os
import sys
from common import common_values
import hybridsearch_embedded
import hybridsearch
import pandas as pd
from hybridsearch.common import ConflictType
import hybridsearch.index as index

current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)
import importlib
from hybridsearch_http import hybridsearch_http


class Testhybridsearch:
    @pytest.fixture(autouse=True)
    def setup_and_teardown(self, local_hybridsearch, http, suffix):
        if local_hybridsearch:
            module = importlib.import_module("hybridsearch_embedded.index")
            globals()["index"] = module
            self.uri = common_values.TEST_LOCAL_PATH
            self.hybridsearch_obj = hybridsearch_embedded.connect(self.uri)
        elif http:
            self.uri = common_values.TEST_LOCAL_HOST
            self.hybridsearch_obj = hybridsearch_http()
        else:
            self.uri = common_values.TEST_LOCAL_HOST
            self.hybridsearch_obj = hybridsearch.connect(self.uri)

        self.suffix = suffix
        yield

        res = self.hybridsearch_obj.disconnect()
        assert res.error_code == hybridsearch.ErrorCode.OK

    def test_simple_add_columns(self):
        table_name = "test_add_column" + self.suffix
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table(table_name, ConflictType.Ignore)
        table_obj = db_obj.create_table(
            table_name,
            {
                "c1": {"type": "int"},
                "c2": {"type": "int"},
            },
        )
        assert table_obj is not None

        res = table_obj.insert([{"c1": 1, "c2": 2}])
        assert res.error_code == hybridsearch.ErrorCode.OK

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame({"c1": [1], "c2": [2]}).astype(
                {"c1": dtype("int32"), "c2": dtype("int32")}
            )
        )

        res = table_obj.add_columns({"c2": {"type": "varchar", "default": "default"}})
        assert res.error_code == hybridsearch.ErrorCode.DUPLICATE_COLUMN_NAME

        res = table_obj.add_columns({"c3": {"type": "varchar"}})
        assert res.error_code == hybridsearch.ErrorCode.NOT_SUPPORTED

        res = table_obj.add_columns({"c3": {"type": "varchar", "default": "default"}})
        assert res.error_code == hybridsearch.ErrorCode.OK

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame({"c1": [1], "c2": [2], "c3": ["default"]}).astype(
                {"c1": dtype("int32"), "c2": dtype("int32"), "c3": dtype("object")}
            )
        )

        table_obj.insert([{"c1": 2, "c2": 3, "c3": "test"}])

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame(
                {"c1": [1, 2], "c2": [2, 3], "c3": ["default", "test"]}
            ).astype(
                {"c1": dtype("int32"), "c2": dtype("int32"), "c3": dtype("object")}
            ),
        )

        db_obj.drop_table(table_name)

    def test_simple_drop_columns(self):
        table_name = "test_drop_column" + self.suffix
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table(table_name, ConflictType.Ignore)
        table_obj = db_obj.create_table(
            table_name,
            {
                "c1": {"type": "int"},
                "c2": {"type": "int"},
                "c3": {"type": "varchar"},
            },
        )
        assert table_obj is not None

        res = table_obj.insert([{"c1": 1, "c2": 2, "c3": "test"}])
        assert res.error_code == hybridsearch.ErrorCode.OK

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame({"c1": [1], "c2": [2], "c3": ["test"]}).astype(
                {"c1": dtype("int32"), "c2": dtype("int32"), "c3": dtype("object")}
            ),
        )

        res = table_obj.drop_columns("c4")
        assert res.error_code == hybridsearch.ErrorCode.COLUMN_NOT_EXIST

        res = table_obj.drop_columns("c2")
        assert res.error_code == hybridsearch.ErrorCode.OK

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame({"c1": [1], "c3": ["test"]}).astype(
                {"c1": dtype("int32"), "c3": dtype("object")}
            ),
        )

        table_obj.insert([{"c1": 2, "c3": "test2"}])

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame({"c1": [1, 2], "c3": ["test", "test2"]}).astype(
                {"c1": dtype("int32"), "c3": dtype("object")}
            ),
        )

        db_obj.drop_table(table_name)

    def test_insert_after_drop_columns(self):
        table_name = "testing_table" + self.suffix
        db_obj = self.hybridsearch_obj.get_database("default_db")

        db_obj.drop_table(table_name, hybridsearch.common.ConflictType.Ignore)

        table_obj = db_obj.create_table(
            table_name,
            {
                "num": {"type": "integer"},
                "body": {"type": "varchar"},
                "vec": {"type": "vector,4,float"},
            },
        )

        table_obj.add_columns(
            {
                "column_name1": {"type": "integer", "default": 0},
                "column_name2": {"type": "float", "default": 0.0},
                "column_name3": {"type": "varchar", "default": ""},
            }
        )
        table_obj.drop_columns(["column_name1"])
        table_obj.insert(
            [
                {
                    "num": 1,
                    "body": "unnecessary and harmful",
                    "vec": [1.0, 1.2, 0.8, 0.9],
                },
                {
                    "num": 2,
                    "body": "Office for Harmful Blooms",
                    "vec": [4.0, 4.2, 4.3, 4.5],
                },
                {
                    "num": 3,
                    "body": "A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
                    "vec": [4.0, 4.2, 4.3, 4.2],
                },
            ]
        )

        result, extra_result = table_obj.output(["*"]).to_df()
        print(result)
        pd.testing.assert_frame_equal(
            result,
            pd.DataFrame(
                {
                    "num": [1, 2, 3],
                    "body": [
                        "unnecessary and harmful",
                        "Office for Harmful Blooms",
                        "A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
                    ],
                    "vec": [
                        [1.0, 1.2, 0.8, 0.9],
                        [4.0, 4.2, 4.3, 4.5],
                        [4.0, 4.2, 4.3, 4.2],
                    ],
                    "column_name2": [0.0, 0.0, 0.0],
                    "column_name3": ["", "", ""],
                }
            ).astype(
                {
                    "num": dtype("int32"),
                    "body": dtype("object"),
                    "vec": dtype("object"),
                    "column_name2": dtype("float32"),
                    "column_name3": dtype("object"),
                }
            ),
        )
        db_obj.drop_table(table_name)
        self.hybridsearch_obj.disconnect()

        print("test done")

    def test_add_drop_column_with_index(self):
        table_name = "test_add_drop_column_with_index" + self.suffix
        db_obj = self.hybridsearch_obj.get_database("default_db")
        db_obj.drop_table(table_name, ConflictType.Ignore)
        table_obj = db_obj.create_table(
            table_name,
            {
                "c1": {"type": "int"},
                "c2": {"type": "int"},
                "c3": {"type": "varchar"},
            },
        )
        assert table_obj is not None

        res = table_obj.create_index(
            "index1", index.IndexInfo("c3", index.IndexType.FullText)
        )
        assert res.error_code == hybridsearch.ErrorCode.OK

        table_obj.insert([{"c1": 1, "c2": 2, "c3": "test"}])

        # res = table_obj.drop_columns("c3")
        # assert res.error_code == hybridsearch.ErrorCode.NOT_SUPPORTED

        res = table_obj.drop_columns("c2")
        assert res.error_code == hybridsearch.ErrorCode.OK

        res = table_obj.add_columns({"c2": {"type": "varchar", "default": "test"}})
        assert res.error_code == hybridsearch.ErrorCode.OK

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame({"c1": [1], "c3": ["test"], "c2": ["test"]}).astype(
                {"c1": dtype("int32"), "c3": dtype("object"), "c2": dtype("object")}
            ),
        )

        table_obj.insert([{"c1": 1, "c2": "t1", "c3": "t2"}])

        res, extra_result = table_obj.output(["*"]).to_df()
        pd.testing.assert_frame_equal(
            res,
            pd.DataFrame(
                {"c1": [1, 1], "c3": ["test", "t2"], "c2": ["test", "t1"]}
            ).astype(
                {"c1": dtype("int32"), "c3": dtype("object"), "c2": dtype("object")}
            ),
        )

        db_obj.drop_table(table_name)
