from setuptools import setup, find_packages

setup(
    name="experiments",
    version="0.1",
    packages=find_packages(where="load_experiments"),
    package_dir={"": "load_experiments"},  # 关键：指定包目录为 src/
)
# pip install -e .  # 以开发者模式安装包