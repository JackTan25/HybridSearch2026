


# import hybridsearch_embedded as hybridsearch
import hybridsearch
import sys

try:
    # Use hybridsearch_embedded module to open a local directory
    # hybridsearch_instance = hybridsearch.connect("/var/hybridsearch")

    #  Use hybridsearch module to connect a remote server
    hybridsearch_instance = hybridsearch.connect(hybridsearch.common.NetworkAddress("127.0.0.1", 23817))

    # 'default_db' is the default database
    db_instance = hybridsearch_instance.get_database("default_db")

    # Drop my_table if it already exists
    db_instance.drop_table("my_table", hybridsearch.common.ConflictType.Ignore)

    # Create a table named "my_table"
    table_instance = db_instance.create_table("my_table", {
        "num": {"type": "integer"},
        "uuid": {"type": "varchar"},
        "score": {"type": "float"},
    })

    # Insert 10 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "uuid": "UUID-1-1",
                "score": 90.5,
            },
            {
                "num": 2,
                "uuid": "UUID-1-2",
                "score": 70.0,
            },
            {
                "num": 3,
                "uuid": "UUID-1-3",
                "score": 75.0,
            },
            {
                "num": 4,
                "uuid": "UUID-1-4",
                "score": 92.0,
            },
            {
                "num": 5,
                "uuid": "UUID-1-5",
                "score": 72.5,
            },
            {
                "num": 6,
                "uuid": "UUID-2-1",
                "score": 69.0,
            },
            {
                "num": 7,
                "uuid": "UUID-2-2",
                "score": 88.0,
            },
            {
                "num": 8,
                "uuid": "UUID-2-3",
                "score": 90.0,
            },
            {
                "num": 9,
                "uuid": "UUID-2-4",
                "score": 86.5,
            },
            {
                "num": 10,
                "uuid": "UUID-2-5",
                "score": 86.0,
            },
        ]
    )

    # Create keyword fulltext index on 'uuid' column
    table_instance.create_index(
        "my_index",
        hybridsearch.index.IndexInfo("uuid", hybridsearch.index.IndexType.FullText, {"analyzer": "keyword"}),
        hybridsearch.common.ConflictType.Error,
    )

    # output 7, 8, 9, 10
    result, extra_result = table_instance.output(["*"]).filter("(score > 80.0) and (score <= 90.0)").to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    # output 6, 8
    result, extra_result = table_instance.output(["*"]).filter("filter_fulltext('uuid', 'UUID-2-1 UUID-2-3')").to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    # output 8
    result, extra_result = table_instance.output(["*"]).filter("(score > 80.0) and (score <= 90.0) and filter_fulltext('uuid', 'UUID-2-1 UUID-2-3')").to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    # drop table
    db_instance.drop_table("my_table")

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
