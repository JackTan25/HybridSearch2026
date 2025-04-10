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

class TestUpdate:
    def test_update(self, cluster: hybridsearchCluster):
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

            db_obj.drop_table(table_name="test_update", conflict_type=ConflictType.Ignore)

            table_obj = db_obj.create_table(
                "test_update", {"c1": {"type": "int", "constraints": ["primary key", "not null"]},
                                "c2": {"type": "int"}, "c3": {"type": "int"}}, ConflictType.Error)
            assert table_obj is not None

            res = table_obj.insert(
                [{"c1": 1, "c2": 10, "c3": 100}, {"c1": 2, "c2": 20, "c3": 200}, {"c1": 3, "c2": 30, "c3": 300},
                {"c1": 4, "c2": 40, "c3": 400}])
            assert res.error_code == ErrorCode.OK

            res = table_obj.update("c1 = 1", {"c2": 90, "c3": 900})
            assert res.error_code == ErrorCode.OK

            res, extra_result = table_obj.output(["*"]).to_df()
            pd.testing.assert_frame_equal(res, pd.DataFrame(
                {'c1': (2, 3, 4, 1), 'c2': (20, 30, 40, 90), 'c3': (200, 300, 400, 900)})
                                        .astype({'c1': dtype('int32'), 'c2': dtype('int32'), 'c3': dtype('int32')}))
            
            time.sleep(1)
            db_obj_2 = hybridsearch2.get_database("default_db")
            table_obj_2 = db_obj_2.get_table("test_update")
            res, extra_result = table_obj_2.output(["*"]).to_df()
            pd.testing.assert_frame_equal(res, pd.DataFrame(
                {'c1': (2, 3, 4, 1), 'c2': (20, 30, 40, 90), 'c3': (200, 300, 400, 900)})
                                        .astype({'c1': dtype('int32'), 'c2': dtype('int32'), 'c3': dtype('int32')}))
        
            res = db_obj.drop_table("test_update", ConflictType.Error)
            assert res.error_code == ErrorCode.OK

    def test_update_on_follower(self, cluster: hybridsearchCluster):
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

            db_obj.drop_table(table_name="test_update", conflict_type=ConflictType.Ignore)

            table_obj = db_obj.create_table(
                "test_update", {"c1": {"type": "int", "constraints": ["primary key", "not null"]},
                                "c2": {"type": "int"}, "c3": {"type": "int"}}, ConflictType.Error)
            assert table_obj is not None

            res = table_obj.insert(
                [{"c1": 1, "c2": 10, "c3": 100}, {"c1": 2, "c2": 20, "c3": 200}, {"c1": 3, "c2": 30, "c3": 300},
                {"c1": 4, "c2": 40, "c3": 400}])
            assert res.error_code == ErrorCode.OK

            time.sleep(1)
            db_obj_2 = hybridsearch2.get_database("default_db")
            table_obj_2 = db_obj_2.get_table("test_update")
            try:
                table_obj_2.update("c1 = 1", {"c2": 90, "c3": 900})
            except hybridsearchException as e:
                print(e)
                assert(e.error_code == 8007)
        
            res = db_obj.drop_table("test_update", ConflictType.Error)
            assert res.error_code == ErrorCode.OK
