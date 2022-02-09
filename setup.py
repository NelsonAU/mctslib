from glob import glob
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        'mctslib._mctslib',
        sorted(glob("src/**/*.cpp")),
        cxx_std=20,
        include_dirs=['src/mctslib/include'],
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
