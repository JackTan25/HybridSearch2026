

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
        "doc": {"type": "varchar"},
    })

    # TODO also show how to import other type of file
    table_instance.import_data(project_directory + "/../test/data/csv/fulltext_delete.csv",
                               {"header": True, "file_type": "csv", "delimiter": "\t"})

    result, extra_result = table_instance.output(["num", "doc"]).to_pl()
    print(result)
    if extra_result is not None:
        print(extra_result)

    hybridsearch_instance.disconnect()

    print('test done')
    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
