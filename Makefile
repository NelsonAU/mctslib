
PYBIND_INCLUDES := $(shell python3 -m pybind11 --includes)
PYBIND_SUFFIX := $(shell python3-config --extension-suffix)
OS := $(shell uname)
OPTIONS = -Wall -shared -std=c++17 -fPIC -fvisibility=hidden
INCLUDES = -I./src/mctslib

ifeq ($(OS),Darwin)
	OPTIONS += -undefined dynamic_lookup
endif

build:
	echo PYBIND_INCLUDES: $(PYBIND_INCLUDES)
	echo PYBIND_SUFFIX: $(PYBIND_SUFFIX)
	c++ $(OPTIONS) $(INCLUDES) $(PYBIND_INCLUDES) src/mctslib/module.cpp -o src/mctslib/mctslib$(PYBIND_SUFFIX)

tests: build
	python3 -m pytest

clean:
	rm src/mctslib/mctslib$(PYBIND_SUFFIX)
	rm -rf src/mctslib.egg-info

compile_commands:
	bear -- c++ $(OPTIONS) $(INCLUDES) $(PYBIND_INCLUDES) src/mctslib/module.cpp -o src/mctslib/mctslib$(PYBIND_SUFFIX)
	



