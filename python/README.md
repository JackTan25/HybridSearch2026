# python-hybridsearch

# update python client

- Update "version" field of [project] chapter and client_version field of ConnectRequest message.
- build new python SDK
- upload to pypi.org
- install new python SDK

Please see [releases.yml](https://github.com/infiniflow/hybridsearch/blob/main/.github/workflows/release.yml) for details.

# using

```python
import hybridsearch
from hybridsearch.common import LOCAL_HOST
from hybridsearch.common import ConflictType

hybridsearch_obj = hybridsearch.connect(LOCAL_HOST)
db = hybridsearch_obj.get_database("default_db")
db.drop_table("my_table", ConflictType.Ignore)
table = db.create_table("my_table", {"num": {"type": "integer"}, "body": {"type": "varchar"}, "vec": {"type": "vector,5,float"}}, ConflictType.Error)
table.insert([{"num": 1, "body": "undesirable, unnecessary, and harmful", "vec": [1.0] * 5}])
table.insert([{"num": 2, "body": "publisher=US National Office for Harmful Algal Blooms", "vec": [4.0] * 5}])
table.insert([{"num": 3, "body": "in the case of plants, growth and chemical", "vec": [7.0] * 5}])

res = table.output(["*"]).match_dense("vec", [3.0] * 5, "float", "ip", 2).to_pl()
print(res)

```

# For developer
```shell
pip install -e .
```
Build hybridsearch-sdk 
```shell
pip install ./python/hybridsearch_sdk 
```
Build the release version of hybridsearch-embedded-sdk in the target location `cmake-build-release`
```shell
pip install . -v --config-settings=cmake.build-type="Release"  --config-settings=build-dir="cmake-build-release"
```
Build the debug version of hybridsearch-embedded-sdk in the target location `cmake-build-debug`
```shell
pip install . -v --config-settings=cmake.build-type="Debug"  --config-settings=build-dir="cmake-build-debug"
```
Note: If you run with the release version and turn jemalloc compile flag on, you must set environment variable, for example
```shell
LD_PRELOAD=$(ldconfig -p | grep 'libjemalloc.so ' | awk '{print $4}') python3 example/simple_example.py
```
Note: If you run with the debug version, you must set the **libasan** environment variable, for example
```shell
LD_PRELOAD=$(find $(clang-18 -print-resource-dir) -name "libclang_rt.asan-x86_64.so") python3 example/simple_example.py
```
Note: When running with the debug version hybridsearch_embedded-sdk, you may find some memory leaks caused by arrow. You can use `ASAN_OPTIONS=detect_leaks=0` to disable memory leak detection, for example
```shell
LD_PRELOAD=$(find $(clang-18 -print-resource-dir) -name "libclang_rt.asan-x86_64.so") ASAN_OPTIONS=detect_leaks=0 python3 example/simple_example.py
```

# run pysdk test
Run a local hybridsearch test in project root directory
```shell
pytest --local-hybridsearch python/test/cases/test_basic.py::Testhybridsearch::test_basic
```
Run a remote hybridsearch test in project root directory
```shell
pytest python/test/cases/test_basic.py::Testhybridsearch::test_basic
```