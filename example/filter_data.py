


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
        "name": {"type": "varchar"},
        "score": {"type": "float"},
    })

    # Insert 10 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "name": "Tom",
                "score": 90.5,
            },
            {
                "num": 2,
                "name": "Henry",
                "score": 70.0,
            },
            {
                "num": 3,
                "name": "James",
                "score": 75.0,
            },
            {
                "num": 4,
                "name": "Toby",
                "score": 92.0,
            },
            {
                "num": 5,
                "name": "Thomas",
                "score": 72.5,
            },
            {
                "num": 6,
                "name": "Charlie",
                "score": 69.0,
            },
            {
                "num": 7,
                "name": "Chris",
                "score": 88.0,
            },
            {
                "num": 8,
                "name": "Bill",
                "score": 90.0,
            },
            {
                "num": 9,
                "name": "Stefan",
                "score": 86.5,
            },
            {
                "num": 10,
                "name": "Steve",
                "score": 86.0,
            },
        ]
    )

    result = table_instance.output(["num", "name", "score"]).filter("(score > 80.0) and (score <= 90.0)").to_pl()
    print(result)

    # result = table_instance.output(["num", "name", "score"]).filter("not (score > 80.0)").to_pl()
    # print(result)

    result, extra_result = table_instance.output(["num", "name", "score"]).filter("num <> 9").to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)
    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)