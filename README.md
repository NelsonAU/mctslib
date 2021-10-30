# mctslib

## Development setup
To do development on this project install a recent version of CMake and do
```
$ pip install -r requirements.txt
$ git submodule init && git submodule update
$ python3 setup.py develep
```

To run tests do
```
$ tox
```

Or to run tests quickly without bothering with venv etc. do
```
$ pytest tests
```
