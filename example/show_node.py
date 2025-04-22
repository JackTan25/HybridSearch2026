

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

    res = hybridsearch_instance.show_current_node()
    print(res)

    sys.exit(0)
except Exception as e:
    print(str(e))
    sys.exit(-1)
