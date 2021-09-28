
PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX := $(shell python3-config --extension-suffix)
OPTIONS = -Wall -shared -std=c++17 -fPIC -fvisibility=hidden
INCLUDES = -I./src

build:
	c++ $(OPTIONS) $(INCLUDES) $(PYBIND_INCLUDES) src/module.cpp -o mctslib$(PYBIND_SUFFIX)

tests: build
	python3 -m pytest

