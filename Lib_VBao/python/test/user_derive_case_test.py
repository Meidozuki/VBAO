import os, sys
import pytest

sys.path.insert(0, os.path.abspath('..'))
import vbao


class NoArgClass:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class WithArgClass:
    def __init__(self, verbose, *args, **kwargs):
        self.verbose = verbose
        super().__init__(*args, **kwargs)


class OverlapClass:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def setProperty(self, key, value):
        pass

    def getProperty(self, key):
        pass


class TestVBAOUserDerive:
    class ModelDerive_MROLeft(vbao.Model, NoArgClass):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ModelDerive_MRORight(NoArgClass, vbao.Model):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewDerive_MROLeft(vbao.View, NoArgClass):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewDerive_MRORight(NoArgClass, vbao.View):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewModelDeriveArg_MROLeft(vbao.ViewModel, WithArgClass):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewModelDeriveArg_MRORight(WithArgClass, vbao.ViewModel):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewModelDeriveConflict_MROLeft(vbao.ViewModel, OverlapClass):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    class ViewModelDeriveConflict_MRORight(OverlapClass, vbao.ViewModel):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)

    # Derive1 -- View/Model (...) -- NoArgsClass -- object
    def test_model_derive_no_arg_succeed(self):
        self.ModelDerive_MROLeft()

    def test_model_derive_no_arg_fail(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ModelDerive_MROLeft(no_use=1)

    def test_view_derive_no_arg_succeed(self):
        self.ViewDerive_MROLeft()

    def test_view_derive_no_arg_fail(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ViewDerive_MROLeft(no_use=1)

    # Derive2 -- NoArgsClass -- View/Model (...) -- object
    def test_model_derive_no_arg_succeed_diff_mro(self):
        self.ModelDerive_MRORight()

    def test_model_derive_no_arg_fail_diff_mro(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ModelDerive_MRORight(no_use=1)

    def test_view_derive_no_arg_succeed_diff_mro(self):
        self.ViewDerive_MRORight()

    def test_view_derive_no_arg_fail_diff_mro(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ViewDerive_MRORight(no_use=1)

    # Derive3 -- ViewModel (...) -- WithArgsClass -- object
    def test_viewmodel_derive_with_arg_succeed(self):
        self.ViewModelDeriveArg_MROLeft(True)

    def test_viewmodel_derive_with_arg_fail(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ViewModelDeriveArg_MROLeft(verbose=True, no_use=1)

    # Derive4 -- WithArgsClass -- ViewModel (...) -- object
    def test_viewmodel_derive_with_arg_succeed_diff_mro(self):
        self.ViewModelDeriveArg_MRORight(True)

    def test_viewmodel_derive_with_arg_fail_diff_mro(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ViewModelDeriveArg_MRORight(verbose=True, no_use=1)

    # Derive5 -- ViewModel (...) -- OverlapClass -- object
    # ps: View's property is not bound
    def test_view_derive_conflict_ctor_succeed(self):
        self.ViewModelDeriveConflict_MROLeft()

    def test_view_derive_conflict_ctor_fail(self):
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            self.ViewModelDeriveConflict_MROLeft(no_use=1)

    def test_view_derive_conflict_set_property_succeed(self):
        v = self.ViewModelDeriveConflict_MROLeft()
        assert '1' not in v.properties
        v.setProperty('1', 2)
        assert '1' in v.properties

    def test_view_derive_conflict_get_property_succeed(self):
        v = self.ViewModelDeriveConflict_MROLeft()
        assert '1' not in v.properties
        v.setProperty('1', 2)
        assert v.getProperty('1') == 2


if __name__ == '__main__':
    pass
