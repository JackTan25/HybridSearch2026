

import argparse
import os
import time

import hybridsearch
from hybridsearch import index
from hybridsearch.common import LOCAL_HOST, LOCAL_hybridsearch_PATH
from hybridsearch.errors import ErrorCode


def import_data(
        data_set: str,
        block_size: int,
        compress: bool,
        opt_topk: int,
        bp_reorder: bool,
        remote: bool,
):
    current_path = os.getcwd()
    data_dir = current_path + "/test/data/benchmark/splade"
    print(f"Data Dir: {data_dir}")

    if data_set == "splade_small":
        import_path = data_dir + "/base_small.csr"
    elif data_set == "splade_1M":
        import_path = data_dir + "/base_1M.csr"
    elif data_set == "splade_full":
        import_path = data_dir + "/base_full.csr"
    else:
        raise Exception("Invalid data set")

    hybridsearch_obj = None
    if remote:
        hybridsearch_obj = hybridsearch.connect(LOCAL_HOST)
    else:
        hybridsearch_obj = hybridsearch.connect(LOCAL_hybridsearch_PATH)
    assert hybridsearch_obj

    db_obj = hybridsearch_obj.get_database("default_db")
    assert db_obj
    db_obj.drop_table("splade_benchmark")
    db_obj.create_table(
        "splade_benchmark", {"col1": {"type": "sparse,30109,float,int16"}}
    )
    table_obj = db_obj.get_table("splade_benchmark")
    assert table_obj

    assert os.path.exists(import_path)
    start = time.time()
    res = table_obj.import_data(import_path, {"file_type": "csr"})
    end = time.time()
    dur = end - start
    print(f"Import splade cost time: {dur} s")
    assert res.error_code == ErrorCode.OK

    start = time.time()
    res = table_obj.create_index(
        "splade_index",
        index.IndexInfo(
            "col1",
            index.IndexType.BMP,
            {"block_size": str(block_size), "compress_type": "compress" if compress else "raww"}
        )
    )
    assert res.error_code == ErrorCode.OK
    end = time.time()
    dur = end - start
    print(f"Create index on splade cost time: {dur} s")

    if opt_topk != 0 or bp_reorder:
        start = time.time()
        options = {}
        if opt_topk != 0:
            options["topk"] = str(opt_topk)
        if bp_reorder:
            options["bp_reorder"] = ""
        res = table_obj.optimize("splade_index", options)
        assert res.error_code == ErrorCode.OK
        end = time.time()
        dur = end - start
        print(f"Optimize index on splade cost time: {dur} s")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Benchmark hybridsearch")
    parser.add_argument(
        "-d",
        "--data",
        type=str,
        default="splade_small",
        dest="data_set",
    )
    parser.add_argument("--block_size", type=int, default=8, dest="block_size")
    parser.add_argument("--compress", type=bool, default=True, dest="compress")
    parser.add_argument("--opt_topk", type=int, default=10, dest="opt_topk")
    parser.add_argument("--bp_reorder", type=bool, default=True, dest="bp_reorder")
    parser.add_argument("-R", "--remote", type=bool, default=True, dest="remote")

    args = parser.parse_args()

    import_data(
        args.data_set,
        args.block_size,
        args.compress,
        args.opt_topk,
        args.bp_reorder,
        args.remote,
    )
