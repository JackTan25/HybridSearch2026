

'''
This example is about connecting local hybridsearch instance, creating table, ing data, importing file into a table, and exporting table's data
'''

import os
current_path = os.path.abspath(__file__)
project_directory = os.path.dirname(current_path)

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
        "age": {"type": "integer"},
        "score": {"type": "float"},
    })

    # Insert 10 rows of data into the 'my_table'
    table_instance.insert(
        [
            {
                "num": 1,
                "name": "Tom",
                "age": 19, 
                "score": 90.5,
            },
            {
                "num": 2,
                "name": "Henry",
                "age": 20, 
                "score": 70.0,
            },
            {
                "num": 3,
                "name": "James",
                "age": 20,
                "score": 75.0,
            },
            {
                "num": 4,
                "name": "Toby",
                "age": 24,
                "score": 92.0,
            },
            {
                "num": 5,
                "name": "Thomas",
                "age": 20,
                "score": 72.5,
            },
            {
                "num": 6,
                "name": "Charlie",
                "age": 20,
                "score": 69.0,
            },
            {
                "num": 7,
                "name": "Chris",
                "age": 21,
                "score": 88.0,
            },
            {
                "num": 8,
                "name": "Bill",
                "age": 21,
                "score": 90.0,
            },
            {
                "num": 9,
                "name": "Stefan",
                "age": 25,
                "score": 86.5,
            },
            {
                "num": 10,
                "name": "Steve",
                "age": 20,
                "score": 86.0,
            },
        ]
    )

    # TODO also show how to export other type of file
    table_instance.export_data(os.getcwd() + "/export_data.jsonl",
                               {"file_type": "jsonl", "offset": 2, "limit": 7, "row_limit": 2}, ["num", "name", "score"])


    hybridsearch_instance.disconnect()
    print('test done')
    sys.exit(0)

except Exception as e:
    print(str(e))
    sys.exit(-1)