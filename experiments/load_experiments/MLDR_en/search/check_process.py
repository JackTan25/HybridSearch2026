import os
import signal
import subprocess
import time
import psutil

import psutil
def terminate_process_tree(pid):
    try:
        parent = psutil.Process(pid)
        for child in parent.children(recursive=True):
            child.terminate()
        parent.terminate()
    except psutil.NoSuchProcess:
        pass

def read_file_content(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()
            return content
    except FileNotFoundError:
        print(f"错误：文件 {file_path} 未找到。")
    except Exception as e:
        print(f"错误：发生未知错误 - {e}")
    return None


def start_service_and_kill(command):
    try:
        # 启动服务
        process = subprocess.Popen(command, shell=True)
        print(f"服务已启动，进程 ID: {process.pid}")

        # 这里可以添加一些逻辑来等待服务完成一些操作，例如等待一段时间
        time.sleep(5)

        # 杀死服务进程
        process.terminate()
        print(f"服务进程 {process.pid} 已被终止。")
    except Exception as e:
        print(f"错误：启动或终止服务时发生错误 - {e}")

def terminate_process_tree(pid):
    try:
        parent = psutil.Process(pid)
        for child in parent.children(recursive=True):
            child.terminate()
        parent.terminate()
    except psutil.NoSuchProcess:
        pass

if __name__ == "__main__":
    # 读取文件内容
    file_path = '/home/ubuntu/hybridsearch/experiments/query_memory_file'
    content = read_file_content(file_path)


    # 启动服务并在适当的时候杀死它
    service_command = "/home/ubuntu/hybridsearch/cmake-build-release/src/hybridsearch -f /home/ubuntu/hybridsearch/conf/hybridsearch_conf.toml"  # 示例命令，启动一个简单的 HTTP 服务器
    process = subprocess.Popen(service_command, shell=True)
    time.sleep(3)
    print(f"服务已启动，进程 ID: {process.pid}")
    print(__file__)
    time.sleep(5)
    terminate_process_tree(process.pid)
    print(f"服务进程 {process.pid} 已被终止。")
    