

'''
This example is to connect local hybridsearch instance, create table, insert data, delete and update data.
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
    print('about to insert data')
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

    result = table_instance.output(["num", "body"]).to_pl()
    print(result)
    print('about to delete data')
    table_instance.delete("num = 2")
    result = table_instance.output(["num", "body"]).to_pl()
    print(result)
    print('about to insert data again')
    table_instance.insert([
        {
            "num": 2,
            "body": r"Office for Harmful Blooms",
            "vec": [4.0, 4.2, 4.3, 4.5],
        },
        {
            "num": 2,
            "body": r"Office for Harmful Blooms",
            "vec": [4.0, 4.2, 4.3, 4.5],
        }
    ])
    result = table_instance.output(["num", "body"]).to_pl()
    print(result)

    print('about to update data')
    table_instance.update("num = 2", {"body": "unnecessary and harmful", "vec": [14.0, 7.2, 0.8, 10.9]})
    result, extra_result = table_instance.output(["*"]).to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    hybridsearch_instance.disconnect()
    print('test done')
    sys.exit(0)

except Exception as e:
    print(str(e))
    sys.exit(-1)
