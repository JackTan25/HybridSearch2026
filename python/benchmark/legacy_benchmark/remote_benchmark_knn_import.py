

import argparse
import os
import time

import hybridsearch
from hybridsearch import index
from hybridsearch.common import LOCAL_HOST, LOCAL_hybridsearch_PATH
from hybridsearch.errors import ErrorCode
from hybridsearch.remote_thrift.client import ThrifthybridsearchClient
from hybridsearch.remote_thrift.table import RemoteTable


def import_data(path, dataset: str, m: int, ef_construction: int, remote: bool):
    print(
        f"dataset: {dataset}, m: {m}, ef_construction: {ef_construction}, remote: {remote}"
    )
    if dataset == "sift_1m":
        import_sift_1m(path + "/sift_base.fvecs", m, ef_construction, remote)
    elif dataset == "gist_1m":
        import_gist_1m(path + "/gist_base.fvecs", m, ef_construction, remote)
    else:
        raise Exception("Invalid data set")


def import_sift_1m(path, m: int, ef_construction: int, remote: bool):
    hybridsearch_obj = None
    if remote:
        hybridsearch_obj = hybridsearch.connect(LOCAL_HOST)
    else:
        hybridsearch_obj = hybridsearch.connect(LOCAL_hybridsearch_PATH)
    assert hybridsearch_obj

    db_obj = hybridsearch_obj.get_database("default_db")
    assert db_obj
    db_obj.drop_table("sift_benchmark")
    db_obj.create_table("sift_benchmark", {"col1": {"type": "vector,128,float"}})
    table_obj = db_obj.get_table("sift_benchmark")
    assert table_obj

    assert os.path.exists(path)

    start = time.time()
    res = table_obj.import_data(path, {"file_type": "fvecs"})
    end = time.time()
    dur = end - start
    print(f"Import sift_1m cost time: {dur} s")

    assert res.error_code == ErrorCode.OK

    start = time.time()
    create_index(table_obj, m, ef_construction, remote)
    end = time.time()
    dur = end - start
    print(f"Create index on sift_1m cost time: {dur} s")


def import_gist_1m(path, m: int, ef_construction: int, remote: bool):
    hybridsearch_obj = None
    if remote:
        hybridsearch_obj = hybridsearch.connect(LOCAL_HOST)
    else:
        hybridsearch_obj = hybridsearch.connect(LOCAL_hybridsearch_PATH)
    assert hybridsearch_obj

    db_obj = hybridsearch_obj.get_database("default_db")
    assert db_obj
    db_obj.drop_table("gist_benchmark")
    db_obj.create_table("gist_benchmark", {"col1": {"type": "vector,960,float"}})
    table_obj = db_obj.get_table("gist_benchmark")
    assert table_obj

    assert os.path.exists(path)

    start = time.time()
    res = table_obj.import_data(path, {"file_type": "fvecs"})
    end = time.time()
    dur = end - start
    print(f"Import gist_1m cost time: {dur} s")

    assert res.error_code == ErrorCode.OK

    start = time.time()
    create_index(table_obj, m, ef_construction, remote)
    end = time.time()
    dur = end - start
    print(f"Create index on gist_1m cost time: {dur} s")


def create_index(table_obj, m: int, ef_construction: int, remote: bool):
    res = table_obj.create_index(
        "hnsw_index",
        index.IndexInfo(
            "col1",
            index.IndexType.Hnsw,
            {
                "m": str(m),
                "ef_construction": str(ef_construction),
                "metric": "l2",
                "encode": "lvq"
            },
        )
    )

    assert res.error_code == ErrorCode.OK


def str2bool(value):
    if isinstance(value, bool):
        return value
    if value.lower() in ("yes", "true", "t", "y", "1"):
        return True
    elif value.lower() in ("no", "false", "f", "n", "0"):
        return False
    else:
        raise argparse.ArgumentTypeError("Boolean value expected")


if __name__ == "__main__":
    current_path = os.getcwd()

    parser = argparse.ArgumentParser(description="Benchmark hybridsearch")

    parser.add_argument(
        "-d",
        "--data",
        type=str,
        default="sift_1m",  # gist_1m
        dest="data_set",
    )
    parser.add_argument("--m", type=int, default=16, dest="m")
    parser.add_argument(
        "--ef_construction", type=int, default=200, dest="ef_construction"
    )
    parser.add_argument("-R", "--remote", type=str2bool, default=True, dest="remote")

    args = parser.parse_args()

    data_dir = current_path + "/test/data/benchmark/" + args.data_set
    print(f"Data Dir: {data_dir}")

    import_data(data_dir, args.data_set, args.m, args.ef_construction, args.remote)
