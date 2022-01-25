from pybind11_tox_example import sub


def test_sub():
    assert sub(2, 2) == 0
