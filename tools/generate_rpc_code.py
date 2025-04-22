import os
import subprocess


def get_path(executable):
    try:
        complete_path = subprocess.check_output(["which", executable])
        return complete_path.decode("utf-8").strip()
    except subprocess.CalledProcessError:
        return None


def create_dir(paths: list):
    for path in paths:
        if not os.path.exists(path):
            os.makedirs(path)


def fix_python_import_path(filename: str):
    # from  import hybridsearch_brpc_pb2 as hybridsearch__brpc__pb2
    # to    from . import hybridsearch_brpc_pb2 as hybridsearch__brpc__pb2
    # replace with your actual file path
    # Read the file
    with open(filename, "r") as file:
        lines = file.readlines()

    for i in range(7 if len(lines) >= 7 else len(lines)):
        line = lines[i]
        words = line.split()
        if len(words) == 4 and words[0] == "import":
            import_module, hybridsearch_brpc_pb2, alias, hybridsearch__brpc__pb2 = words
            # Construct the new import line.
            new_line = f"from . {import_module} {hybridsearch_brpc_pb2} {alias} {hybridsearch__brpc__pb2}\n"
            lines[i] = new_line  # replace the line

        # Write back to the file
    with open(filename, "w") as file:
        file.writelines(lines)


def generate_thrift():
    hybridsearch_proj_dir = os.getcwd()
    python_dir = hybridsearch_proj_dir + "/python/hybridsearch_sdk/hybridsearch/remote_thrift"
    cpp_dir = hybridsearch_proj_dir + "/src/network/hybridsearch_thrift"
    peer_server_cpp_dir = hybridsearch_proj_dir + "/src/network/peer_server_thrift"
    create_dir([python_dir, cpp_dir, peer_server_cpp_dir])
    hybridsearch_thrift_file = hybridsearch_proj_dir + "/thrift/hybridsearch.thrift"
    peer_server_file = hybridsearch_proj_dir + "/thrift/peer_server.thrift"
    cmds = [
        "thrift --version",
        f"thrift --out {python_dir} --gen py {hybridsearch_thrift_file}",
        f"thrift -r --out {cpp_dir} --gen cpp:no_skeleton {hybridsearch_thrift_file}",
        f"thrift -r --out {peer_server_cpp_dir} --gen cpp:no_skeleton {peer_server_file}",
    ]
    for cmd in cmds:
        print(cmd)
        os.system(cmd)


if __name__ == "__main__":
    generate_thrift()
