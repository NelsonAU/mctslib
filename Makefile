
PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX := $(shell python3-config --extension-suffix)
OPTIONS = -Wall -shared -std=c++17 -fPIC -fvisibility=hidden
INCLUDES = -I./src/mctslib

build:
	echo PYBIND_INCLUDES: $(PYBIND_INCLUDES)
	echo PYBIND_SUFFIX: $(PYBIND_SUFFIX)
	c++ $(OPTIONS) $(INCLUDES) $(PYBIND_INCLUDES) src/mctslib/module.cpp -o src/mctslib$(PYBIND_SUFFIX)

tests: build
	python3 -m pytest

