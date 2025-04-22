from abc import abstractmethod
import logging
import os
import subprocess
import os
import psutil
import time
import hybridsearch
from hybridsearch.common import ConflictType, hybridsearchException
from hybridsearch.errors import ErrorCode

PYTEST_LOG_FILE = "restart_test.py.log"

class hybridsearchRunner:
    def __init__(self, hybridsearch_path: str, *, logger=None):
        self.data_dir = "/var/hybridsearch"
        self.default_config_path = "./conf/hybridsearch_conf.toml"
        self.script_path = "./scripts/timeout_kill.sh"
        self.hybridsearch_path = hybridsearch_path

        if not os.access(self.hybridsearch_path, os.X_OK):
            raise Exception(f"{self.hybridsearch_path} is not executable.")

        self.i = 0
        if logger is not None:
            self.logger = logger
            return
        self.logger = logging.getLogger("hybridsearch_runner")
        if not self.logger.handlers:
            self.logger.setLevel(logging.DEBUG)
            handler = logging.StreamHandler()
            handler.setLevel(logging.INFO)
            formatter = logging.Formatter(
                "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
            )
            handler.setFormatter(formatter)
            self.logger.addHandler(handler)

            handler = logging.FileHandler(PYTEST_LOG_FILE, delay=True)
            handler.setLevel(logging.DEBUG)
            handler.setFormatter(formatter)
            self.logger.addHandler(handler)

    def clear(self):
        os.system(
            f"rm -rf {self.data_dir}/data {self.data_dir}/log {self.data_dir}/persistence {self.data_dir}/tmp {self.data_dir}/wal"
        )
        os.system(f"rm -rf restart_test.log.*")
        os.system(f"rm -rf {PYTEST_LOG_FILE}")
        print(f"clear {self.data_dir}")
        self.i = 0

    def init(self, config_path: str | None = None):
        init_timeout = 60
        if config_path is None:
            config_path = self.default_config_path
        cmd = f"{self.hybridsearch_path} --config={config_path} > restart_test.log.{self.i} 2>&1"

        pids = [
            proc.pid
            for proc in psutil.process_iter(["pid", "name"])
            if "hybridsearch" in proc.info["name"]
        ]
        if len(pids) > 0:
            ret = os.system(
                f"bash {self.script_path} {init_timeout} {' '.join(map(str, pids))}"
            )
            if ret != 0:
                raise Exception("An error occurred.")

        # unset env LD_PRELOAD, ASAN_OPTIONS
        my_env = os.environ.copy()
        my_env["LD_PRELOAD"] = ""
        my_env["ASAN_OPTIONS"] = ""
        self.process = subprocess.Popen(cmd, shell=True, env=my_env)
        self.i += 1

    def uninit(self, kill: bool = False, timeout: int = 60):
        if self.process is None:
            return
        pids = []
        for child in psutil.Process(self.process.pid).children(recursive=True):
            pids.append(child.pid)
        if len(pids) == 0:
            raise Exception("Cannot find hybridsearch process.")

        if kill:
            os.system(f"kill -9 {' '.join(map(str, pids))}")
            time.sleep(1)
            while any(psutil.pid_exists(pid) for pid in pids):
                time.sleep(1)
            self.process = None
            return

        ret = os.system(f"bash {self.script_path} {timeout} {' '.join(map(str, pids))}")
        if ret != 0:
            raise Exception("An error occurred.")
        self.process = None

    def connected(self):
        return self.process is not None

    def connect(self, uri: str):
        try_n = 100
        time.sleep(0.5)
        hybridsearch_obj = None
        for i in range(try_n):
            try:
                if hybridsearch_obj is None:
                    hybridsearch_obj = hybridsearch.connect(uri, self.logger)
                ret = hybridsearch_obj.get_database("default_db")
                break
            except Exception as e:
                if hybridsearch_obj is not None:
                    if isinstance(e, hybridsearchException):
                        if e.error_code == ErrorCode.hybridsearch_IS_INITING:
                            print("wait hybridsearch starting")
                            time.sleep(0.5)
                        else:
                            raise e
                    else:
                        raise e
                else:
                    print(e)
                    time.sleep(0.5)
                print(f"retry connect {i}")
        else:
            raise Exception(f"Cannot connect to hybridsearch after {try_n} retries.")
        return hybridsearch_obj


def hybridsearch_runner_decorator_factory(
    config_path: str | None,
    uri: str,
    hybridsearch_runner: hybridsearchRunner,
    *,
    shutdown_out: bool = False,
    kill: bool = False,
    terminate_timeout: int = 60,
    check_kill: bool = True
):
    def decorator(f):
        def wrapper(*args, **kwargs):
            hybridsearch_runner.init(config_path)
            hybridsearch_obj = hybridsearch_runner.connect(uri)
            try:
                return f(hybridsearch_obj, *args, **kwargs)
            finally:
                try:
                    hybridsearch_obj.disconnect()
                except Exception:
                    if not shutdown_out:
                        raise
                try:
                    hybridsearch_runner.uninit(kill, terminate_timeout)
                except Exception:
                    if check_kill:
                        raise

        return wrapper

    return decorator
