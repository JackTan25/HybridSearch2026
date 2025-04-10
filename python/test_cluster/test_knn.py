import time
import os
from numpy import dtype
import pandas as pd
import pytest
from hybridsearch_cluster import hybridsearchCluster
from hybridsearch.common import ConflictType
from hybridsearch.common import hybridsearchException
from hybridsearch.errors import ErrorCode
import common_values

class TestKnn:
    def test_knn(self, cluster: hybridsearchCluster):
        with cluster:
            cluster.add_node("node1", "conf/leader.toml")
            cluster.add_node("node2", "conf/follower.toml")

            print("init nodes")
            cluster.set_leader("node1")
            cluster.set_follower("node2")
            time.sleep(1)

            hybridsearch1 = cluster.client("node1")
            hybridsearch2 = cluster.client("node2")

            db_obj = hybridsearch1.get_database("default_db")

            test_csv_dir = common_values.TEST_DATA_DIR + "csv/tmp_20240116.csv"
            print(f"import file: {test_csv_dir}")
            assert os.path.exists(test_csv_dir)
            if not os.path.exists(common_values.TEST_TMP_DIR):
                os.makedirs(common_values.TEST_TMP_DIR)

            db_obj.drop_table("test_knn", ConflictType.Ignore)
            table_obj = db_obj.create_table("test_knn", {
            "variant_id": {"type": "varchar"},
            "gender_vector": {"type": "vector,4,float"},
            "color_vector": {"type": "vector,4,float"},
            "category_vector": {"type": "vector,4,float"},
            "tag_vector": {"type": "vector,4,float"},
            "other_vector": {"type": "vector,4,float"},
            "query_is_recommend": {"type": "varchar"},
            "query_gender": {"type": "varchar"},
            "query_color": {"type": "varchar"},
            "query_price": {"type": "float"}
            }, ConflictType.Error)
            res = table_obj.import_data(test_csv_dir, None)
            assert res.error_code == ErrorCode.OK

            res, extra_result = table_obj.output(["variant_id", "_row_id"]).match_dense("gender_vector", [1.0] * 4, "float", "ip", 10).to_pl()
            print(res)

            time.sleep(1)
            db_obj_2 = hybridsearch2.get_database("default_db")
            table_obj_2 = db_obj_2.get_table("test_knn")
            res, extra_result = table_obj_2.output(["variant_id", "_row_id"]).match_dense("gender_vector", [1.0] * 4, "float", "ip", 10).to_pl()
            print(res)

            res = db_obj.drop_table("test_knn", ConflictType.Error)
            assert res.error_code == ErrorCode.OK
