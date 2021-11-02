# mctslib

## Development setup
To do development on this project install a recent version of CMake and do
```sh
$ pip install -r requirements.txt
$ git submodule init && git submodule update
$ python3 setup.py develop # setup.py will build and install development version
```

To run tests do
```sh
$ tox
```

Or to run tests quickly without bothering with venv etc. do
```sh
$ pytest tests
```
