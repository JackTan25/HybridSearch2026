# Examples

## Python SDK examples

The Python scripts in the current directory provides the examples of the hybridsearch SDK API. Before running these scripts, please ensure that the correct version of hybridsearch SDK is installed. If you are using the server version of hybridsearch, you can use the following command to install it

```shell
pip install hybridsearch-sdk==0.6.0.dev3
```

To use hybridsearch locally as a Python module, install the hybridsearch Embedded SDK:

```shell
pip install hybridsearch-embedded-sdk==0.6.0.dev3
```

Once the installation completes, run some simple examples:

```shell
python simple_example.py
```

## HTTP examples

We provide several SHELL scripts in the **/example/http** directory, and you need to install `curl` to run these scripts. For Debian or Ubuntu users, use the following command:

```shell
sudo apt install curl
```

The HTTP APIs work only with the hybridsearch server. Ensure that an hybridsearch server is running properly before executing these scripts:

```shell
bash http/create_list_show_database.sh
```