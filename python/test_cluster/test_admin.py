import pytest
from hybridsearch_cluster import hybridsearchCluster
from hybridsearch.common import hybridsearchException


def test_admin(cluster: hybridsearchCluster):
    with cluster:
        logger = cluster.logger
        logger.info("test_admin")
        cluster.add_node("test", "conf/leader.toml")
        hybridsearch1 = cluster.client("test")
        assert hybridsearch1 is not None
        res = hybridsearch1.show_current_node()
        logger.info(f'{res.node_role}, {res.node_status}')
        assert (res.node_role == "admin")
        assert (res.node_status == "started")

        res = hybridsearch1.show_admin_variables()
        logger.info(res.data)

        res = hybridsearch1.show_admin_configs()
        logger.info(res.data)

        res = hybridsearch1.show_admin_catalogs()
        logger.info(res.data)

        res = hybridsearch1.show_admin_logs()
        logger.info(res.data)

        cluster.remove_node("test")


def test_admin_fail0(cluster: hybridsearchCluster):
    with cluster:
        logger = cluster.logger
        logger.info("test_admin_fail0")
        cluster.add_node("test", "conf/leader.toml")
        hybridsearch1 = cluster.client("test")
        assert hybridsearch1 is not None

        with pytest.raises(hybridsearchException) as e:
            hybridsearch1.show_node('test')
        logger.info(e.value)

        with pytest.raises(hybridsearchException) as e:
            hybridsearch1.list_nodes()
        logger.info(e.value)

        cluster.remove_node("test")
