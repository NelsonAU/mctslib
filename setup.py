from glob import glob
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension


ext_modules = [
    Pybind11Extension(
        'mctslib.mctslibcpp',
        sorted(glob("src/**/*.cpp")),
        include_dirs=['src/'],
        extra_compile_args=['-std=c++20']
    ),
]


setup(
    name='mctslib',
    version='0.1',
    author='David Dunleavy',
    author_email='dunleavyd14@gmail.com',
    description='',
    long_description='',
    packages=find_packages('src'),
    package_dir={'': 'src'},
    ext_modules=ext_modules,
    install_requires=["pybind11"]
)
