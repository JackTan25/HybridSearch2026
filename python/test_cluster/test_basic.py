import time

import pytest
from hybridsearch_cluster import hybridsearchCluster
from mocked_hybridsearch_cluster import MockhybridsearchCluster
import time


def test_admin(cluster: hybridsearchCluster):
    with cluster:
        cluster.add_node("test", "conf/leader.toml")
        hybridsearch1 = cluster.client("test")
        assert hybridsearch1 is not None
        res = hybridsearch1.show_current_node()
        print(res.node_name)
        assert (res.node_role == "admin")


def test_standalone(cluster: hybridsearchCluster):
    with cluster:
        cluster.add_node("test", "conf/pytest_parallel_hybridsearch_conf.toml")
        cluster.set_standalone("test")
        test_client = cluster.client("test")
        assert test_client is not None

        test_client.create_database("db1")
        test_client.drop_database("db1")


def test_0(cluster: hybridsearchCluster):
    with cluster:
        cluster.add_node("node1", "conf/leader.toml")
        cluster.add_node("node2", "conf/follower.toml")

        cluster.set_leader("node1")
        cluster.set_follower("node2")

        time.sleep(1)

        cluster.remove_node("node2")
        cluster.remove_node("node1")


# def test_mock(mock_cluster: MockhybridsearchCluster):
#     cluster = mock_cluster
#     with cluster:
#         cluster.add_node("node1", "conf/leader.toml")
#         cluster.add_node("node2", "conf/follower.toml")

#         cluster.set_leader("node1")
#         cluster.set_follower("node2")

#         time.sleep(1)

#         cluster.disconnect("node2")
#         time.sleep(0.1)
#         cluster.reconnect("node2")

#         cluster.block_peer_net("node2")
#         time.sleep(0.1)
#         cluster.restore_peer_net("node2")

#         time.sleep(1)

#         cluster.remove_node("node2")
#         cluster.remove_node("node1")


# @pytest.mark.docker
# def test_docker(docker_cluster: DockerhybridsearchCluster):
#     cluster = docker_cluster

#     cluster.add_node("node1", "conf/leader.toml")
#     cluster.add_node("node2", "conf/follower.toml")

#     print("init nodes")

#     cluster.set_leader("node1")
#     cluster.set_follower("node2")

#     time.sleep(1)

#     cluster.disconnect("node2")
#     time.sleep(0.1)
#     cluster.reconnect("node2")

#     res = cluster.client("node1").list_databases()
#     print(res.db_names)

#     time.sleep(1)

#     print("remove nodes")

#     cluster.clear()

# # n1 admin, n2 admin, n2 connect n1 as follower-> failed, n2 connect n1 as learner-> failed
# # n1 leader, n2 follower, n3 connect n2 as follower-> failed, n3 connect n2 as learner-> failed
# #
