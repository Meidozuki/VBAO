import logging
import pytest

from test_util import vbao


# test Qt compatible
try:
    # todo: now only test under pyside6
    from PySide6.QtCore import QObject
except ImportError:
    pytest.skip("Qt not installed", allow_module_level=True)


class TestQtCompatible:
    class ViewModelDerive_MROLeft(vbao.ViewModel, QObject):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewModelDerive_MRORight(QObject, vbao.ViewModel):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    # Derive1 -- ViewModel (...) -- QObject -- object
    def test_viewmodel_derive_no_arg_succeed(self):
        self.ViewModelDerive_MROLeft()
        print('\n',self.ViewModelDerive_MROLeft.mro(),sep='')

    def test_viewmodel_derive_no_arg_fail(self):
        with pytest.raises(AttributeError, match=r"PySide6.QtCore.QObject\.__init__\(\): 'no_use' is not a Qt property or a signal"):
            self.ViewModelDerive_MROLeft(no_use=1)

    # Derive2 -- QObject -- ViewModel (...) -- object
    def test_viewmodel_derive_no_arg_succeed_diff_mro(self):
        self.ViewModelDerive_MRORight()
        print('\n',self.ViewModelDerive_MRORight.mro(),sep='')

    def test_viewmodel_derive_no_arg_fail_diff_mro(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ViewModelDerive_MRORight(no_use=1)
