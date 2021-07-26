
PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX := $(shell python3-config --extension-suffix)

build:
	c++ -Wall -shared -std=c++11 -fPIC -fvisibility=hidden $(PYBIND_INCLUDES) module.cpp -o mctslib$(PYBIND_SUFFIX)

tests: build
	python3 tests.py

