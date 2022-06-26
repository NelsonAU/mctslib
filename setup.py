from glob import glob
from pathlib import Path
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension
import os


def find_ext_modules(**kwargs):
    """Creates list of Pybind11Extensions and passes extra arguments to them"""
    return [Pybind11Extension(f'mctslib.{Path(p).stem}', [p], **kwargs)
            for p in glob('src/**/*mcts.cpp')]


ext_options = {
    'cxx_std': 20,
    'include_dirs': ['src/mctslib/cpp'],
    'define_macros': [('MCTSLIB_USING_PYBIND11', None)]
}

if 'MCTSLIB_CXX_FLAGS' in os.environ:
    del ext_options['cxx_std']
    ext_options['extra_compile_args'] = os.environ['MCTSLIB_CXX_FLAGS'].split(' ')
    print(ext_options)


setup(
    name='mctslib',
    version='0.1',
    author='David Dunleavy',
    author_email='dunleavyd14@gmail.com',
    description='',
    long_description='',
    packages=find_packages('src'),
    package_dir={'': 'src'},
    ext_modules=find_ext_modules(**ext_options),
    install_requires=['pybind11']
)
