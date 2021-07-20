
PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX := $(shell python3-config --extension-suffix)

build:
	c++ -Wall -shared -std=c++11 -fPIC $(PYBIND_INCLUDES) module.cpp -o example$(PYBIND_SUFFIX)

test: build
	python3 tests.py

