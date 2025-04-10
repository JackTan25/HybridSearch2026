

"""
This example is to connect local hybridsearch instance, create table, insert data, search the data
"""

# import hybridsearch_embedded as hybridsearch
import hybridsearch
import sys

try:
    # open a local directory(default = "/var/hybridsearch") or connect to server(default = NetworkAddress("127.0.0.1", 23817)) to store the data
    hybridsearch_instance = hybridsearch.connect()

    # 'default_db' is the default database
    db_instance = hybridsearch_instance.get_database("default_db")

    # Drop my_table if it already exists
    db_instance.drop_table("my_table", hybridsearch.common.ConflictType.Ignore)

    # Create a table named "my_table"
    table_instance = db_instance.create_table(
        "my_table",
        {
            "num": {"type": "integer"},
            "body": {"type": "varchar"},
            "vec": {"type": "vector, 4, float"},
            "sparse": {"type": "sparse,100,float,int"},
            "year": {"type": "integer"},
            "tensor": {"type": "tensor,4,float"},
        },
    )

    # Insert 4 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "body": r"unnecessary and harmful",
                "vec": [1.0, 1.2, 0.8, 0.9],
                "sparse": hybridsearch.common.SparseVector([10, 20, 30], [1.1, 2.2, 3.3]),
                "year": 2024,
                "tensor": [[1.0, 0.0, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]],
            },
            {
                "num": 2,
                "body": r"Office for Harmful Blooms",
                "vec": [4.0, 4.2, 4.3, 4.5],
                "sparse": hybridsearch.common.SparseVector([40, 50, 60], [4.4, 5.5, 6.6]),
                "year": 2023,
                "tensor": [[4.0, 0.0, 4.3, 4.5], [4.0, 4.2, 4.4, 5.0]],
            },
            {
                "num": 3,
                "body": r"A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
                "vec": [4.0, 4.2, 4.3, 4.2],
                "sparse": hybridsearch.common.SparseVector([70, 80, 90], [7.7, 8.8, 9.9]),
                "year": 2019,
                "tensor": [[0.9, 0.1, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]],
            },
            {
                "num": 4,
                "body": r"The American Football Conference (AFC) harm chemical anarchism add test is one of harm chemical the two conferences of the National Football League (NFL). This add test conference and its counterpart, the National Football Conference (NFC), currently contain 16 teams each, making up the 32 teams of the NFL. The current AFC title holder is the New England Patriots.",
                "vec": [4.0, 4.2, 4.3, 4.5],
                "sparse": hybridsearch.common.SparseVector([20, 80, 90], [7.7, 7.8, 97.9]),
                "year": 2018,
                "tensor": [[4.0, 4.2, 4.3, 4.5], [4.0, 4.2, 4.3, 4.4]],
            },
        ]
    )

    # TODO: dense vector + sparse vector + full-text + structured data filter + tensor reranker
    # result = table_instance.output(["num", "body"]).match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "ip", 3).match_text("body", "blooms","topn=1").fusion(method="rrf").to_pl()

    res = table_instance.create_index(
        "my_index",
        hybridsearch.index.IndexInfo("body", hybridsearch.index.IndexType.FullText),
        hybridsearch.common.ConflictType.Error,
    )

    result, extra_result = (
        table_instance.output(
            ["num", "body", "vec", "sparse", "year", "tensor", "score()"]
        )
        .match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "cosine", 3)
        .match_sparse(
            "sparse", hybridsearch.common.SparseVector([0, 20, 80], [1.0, 2.0, 3.0]), "ip", 3
        )
        .match_text("body", "blooms", 10)
        .filter("year < 2024")
        .fusion(
            method="match_tensor", topn=3,
            fusion_params={"field": "tensor", "element_type": "float",
                           "query_tensor": [[0.9, 0.0, 0.0, 0.0], [1.1, 0.0, 0.0, 0.0]]}
        )
        .to_pl()
        # .explain(explain_type=hybridsearch.table.ExplainType.UnOpt)
    )
    if extra_result is not None:
        print(extra_result)

    print(result)
    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
