from pybind11_tox_example import double


def test_double():
    assert double(2) == 4
