from pybind11_tox_example import add


def test_add():
    assert add(2, 2) == 4
