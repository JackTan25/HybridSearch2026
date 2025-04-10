

'''
This example is to connect local hybridsearch instance, create table, insert data, search the data
'''

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
        "body": {"type": "varchar"},
        "vec": {"type": "vector, 4, float"},
    })

    # Insert 3 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "body": r"unnecessary and harmful",
                "vec": [1.0, 1.2, 0.8, 0.9],
            },
            {
                "num": 2,
                "body": r"Office for Harmful Blooms",
                "vec": [4.0, 4.2, 4.3, 4.5],
            },
            {
                "num": 3,
                "body": r"A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
                "vec": [4.0, 4.2, 4.3, 4.2],
            },
        ]
    )

    result, extra_result = table_instance.output(["num", "vec", "_similarity"]).match_dense("vec", [3.0, 2.8, 2.7, 3.1], "float", "cosine", 3).limit(2).offset(1).option({"total_hits_count": True}).to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
