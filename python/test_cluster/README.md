# Prerequisite

```bash
sudo apt install bridge-utils
```

## Run all cluster test cases
python3 tools/run_cluster_test.py --hybridsearch_path=cmake-build-debug/src/hybridsearch --hybridsearch_dir=/var/hybridsearch

## Run specific test case
python3 tools/run_cluster_test.py --test_case=test_basic.py::test_admin --hybridsearch_path=cmake-build-debug/src/hybridsearch --hybridsearch_dir=/var/hybridsearch