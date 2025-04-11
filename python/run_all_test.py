

import os
import pytest 

def run_sdk(): 
    os.system("cd test")
    os.system("python3 -m pytest -m 'not complex and not slow' test/test_database.py")

def run_http():
    os.system("cd test_http_api")
    os.system("python3 -m pytest -m 'not complex and not slow' test_http_api/test_update.py")   

if __name__ == '__main__':
    run_http()
    #run_http()
