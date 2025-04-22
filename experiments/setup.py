from setuptools import setup, find_packages

setup(
    name="experiments",
    version="0.1",
    packages=find_packages(where="load_experiments"),
    package_dir={"": "load_experiments"},  #  src/
)
# pip install -e .  # 