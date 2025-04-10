

'''
This example is about connecting to the local hybridsearch instance, creating table, inserting data, and searching data
'''

# import hybridsearch_embedded as hybridsearch
import hybridsearch
import sys

try:
    # Use hybridsearch_embedded module to open a local directory
    # hybridsearch_instance = hybridsearch.connect("/var/hybridsearch")

    #  Use hybridsearch module to connect a remote server
    hybridsearch_instance = hybridsearch.connect(hybridsearch.common.NetworkAddress("127.0.0.1", 23817))

    hybridsearch_instance.create_database("db1", hybridsearch.common.ConflictType.Ignore, "database1")
    hybridsearch_instance.create_database("db2", hybridsearch.common.ConflictType.Ignore, "database2")
    hybridsearch_instance.create_database("db3", hybridsearch.common.ConflictType.Ignore, "database3")

    res = hybridsearch_instance.list_databases()
    print(res.db_names)

    # 'default_db' is the default database
    db_instance = hybridsearch_instance.get_database("db1")

    table1_object = db_instance.create_table("table1", {
        "num": {"type": "integer", "constraints": ["PRIMARY KEY"], "comment": "number column"},
        "body": {"type": "varchar", "comment": "body column"},
        "vec": {"type": "vector, 4, float", "comment": "vec column"},
    }, hybridsearch.common.ConflictType.Ignore)

    db_instance.create_table("table2", {
        "num": {"type": "integer", "constraints": ["PRIMARY KEY"]},
        "body": {"type": "varchar"},
        "vec": {"type": "vector, 4, float"},
    }, hybridsearch.common.ConflictType.Ignore)

    db_instance.create_table("table3", {
        "num": {"type": "integer", "constraints": ["PRIMARY KEY"]},
        "body": {"type": "varchar"},
        "vec": {"type": "vector, 4, float"},
    }, hybridsearch.common.ConflictType.Ignore)

    res = db_instance.list_tables()
    print(res.table_names)

    res = table1_object.show_columns()
    print(res)
    # # Insert 3 rows of data into the 'my_table'
    # table_instance.insert(
    #     [
    #         {
    #             "num": 1,
    #             "body": r"unnecessary and harmful",
    #             "vec": [1.0, 1.2, 0.8, 0.9],
    #         },
    #         {
    #             "num": 2,
    #             "body": r"Office for Harmful Blooms",
    #             "vec": [4.0, 4.2, 4.3, 4.5],
    #         },
    #         {
    #             "num": 3,
    #             "body": r"A Bloom filter is a space-efficient probabilistic data structure, conceived by Burton Howard Bloom in 1970, that is used to test whether an element is a member of a set.",
    #             "vec": [4.0, 4.2, 4.3, 4.5],
    #         },
    #     ]
    # )
    #
    # res = table_instance.output(["num", "body", "vec"]).to_pl()
    # print(res)

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
