import subprocess
import argparse
import sys
import os

python_executable = sys.executable


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Python restart test for hybridsearch")
    parser.add_argument(
        "--hybridsearch_path",
        type=str,
        default="./build/Debug/src/hybridsearch",
    )
    parser.add_argument(
        "--slow",
        type=bool,
        default=False,
    )
    parser.add_argument(
        "--test_case",
        type=str,
        required=False,
    )

    args = parser.parse_args()
    hybridsearch_path = args.hybridsearch_path
    slow = args.slow

    current_path = os.getcwd()
    python_test_dir = current_path + "/python"

    test_case = ""
    if args.test_case:
        test_case = f"{python_test_dir}/restart_test/{args.test_case}"
    else:
        test_case = f"{python_test_dir}/restart_test"

    if not slow:
        process = subprocess.Popen(
            [
                python_executable,
                "-m",
                "pytest",
                "-v",
                test_case,
                f"--hybridsearch_path={hybridsearch_path}",
                "-x",
                "-s",
                "-m",
                "not slow",
                # "-k",
                # "test_optimize_from_different_database"
            ]
        )
    else:
        process = subprocess.Popen(
            [
                python_executable,
                "-m",
                "pytest",
                "-v",
                test_case,
                f"--hybridsearch_path={hybridsearch_path}",
                "-x",
                "-s",
            ]
        )
    process.wait()
    if process.returncode != 0:
        print(f"An error occurred: {process.stderr}")
        sys.exit(-1)
