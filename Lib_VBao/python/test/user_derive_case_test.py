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


class CollideClass:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def setProperty(self, key, value):
        pass

    def getProperty(self, key):
        pass


base = {'Model': vbao.Model, 'ViewModel': vbao.ViewModel, 'View': vbao.View}
user_class = {'NoArg': NoArgClass, 'WithArg': WithArgClass, 'Collide': CollideClass}
mro_suffix = {True: '_MROLeft', False: '_MRORight'}


def makeClass(vbao_base: str, other: str, mro_left=True):
    name = ''.join([vbao_base, 'Derive', other, mro_suffix[mro_left]])
    if mro_left:
        return type(name, (base[vbao_base], user_class[other]), {})
    else:
        return type(name, (user_class[other], base[vbao_base]), {})


class TestBasicDerive:
    # Derive1 -- View/Model (...) -- NoArgsClass -- object
    def test_model_derive_no_arg_succeed(self):
        cls = makeClass('Model', 'NoArg')
        assert cls.__name__ == 'ModelDeriveNoArg_MROLeft'
        cls()

    def test_model_derive_no_arg_fail(self):
        cls = makeClass('Model', 'NoArg')
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(no_use=1)

    def test_viewmodel_derive_no_arg_succeed(self):
        cls = makeClass('ViewModel', 'NoArg')
        assert cls.__name__ == 'ViewModelDeriveNoArg_MROLeft'
        cls()

    def test_viewmodel_derive_no_arg_fail(self):
        cls = makeClass('ViewModel', 'NoArg')
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(no_use=1)

    def test_view_derive_no_arg_succeed(self):
        cls = makeClass('View', 'NoArg')
        assert cls.__name__ == 'ViewDeriveNoArg_MROLeft'
        cls()

    def test_view_derive_no_arg_fail(self):
        cls = makeClass('View', 'NoArg')
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(no_use=1)

    # Derive2 -- NoArgsClass -- View/Model (...) -- object
    def test_model_derive_no_arg_succeed_diff_mro(self):
        cls = makeClass('Model', 'NoArg', False)
        assert cls.__name__ == 'ModelDeriveNoArg_MRORight'
        cls()

    def test_model_derive_no_arg_fail_diff_mro(self):
        cls = makeClass('Model', 'NoArg', False)
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(no_use=1)

    def test_viewmodel_derive_no_arg_succeed_diff_mro(self):
        cls = makeClass('ViewModel', 'NoArg', False)
        assert cls.__name__ == 'ViewModelDeriveNoArg_MRORight'
        cls()

    def test_viewmodel_derive_no_arg_fail_diff_mro(self):
        cls = makeClass('ViewModel', 'NoArg', False)
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(no_use=1)

    def test_view_derive_no_arg_succeed_diff_mro(self):
        cls = makeClass('View', 'NoArg', False)
        assert cls.__name__ == 'ViewDeriveNoArg_MRORight'
        cls()

    def test_view_derive_no_arg_fail_diff_mro(self):
        cls = makeClass('View', 'NoArg', False)
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(no_use=1)


class TestVBAOUserDerive:
    # Derive3 -- ViewModel (...) -- WithArgsClass -- object
    def test_viewmodel_derive_with_arg(self):
        cls = makeClass('ViewModel', 'WithArg')
        assert cls.__name__ == 'ViewModelDeriveWithArg_MROLeft'
        # success
        cls(True)
        # error
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(verbose=True, no_use=1)

    # Derive4 -- WithArgsClass -- ViewModel (...) -- object
    def test_viewmodel_derive_with_arg_diff_mro(self):
        cls = makeClass('ViewModel', 'WithArg', False)
        assert cls.__name__ == 'ViewModelDeriveWithArg_MRORight'
        # success
        cls(True)
        # error
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(verbose=True, no_use=1)

    # Derive5 -- ViewModel (...) -- OverlapClass -- object
    # ps: View's property is not bound
    def test_viewmodel_derive_conflict_ctor(self):
        cls = makeClass('ViewModel', 'Collide')
        assert cls.__name__ == 'ViewModelDeriveCollide_MROLeft'
        # success
        cls()
        # error
        with pytest.raises(TypeError, match=r"object\.__init__\(\) takes exactly one argument"):
            cls(verbose=True, no_use=1)

    mixin_enabled = True

    @pytest.mark.skipif(not mixin_enabled, reason='')
    def test_viewmodel_derive_conflict_set_property(self):
        v = makeClass('ViewModel', 'Collide')()
        assert '1' not in v.properties
        v.setProperty('1', 2)
        assert '1' in v.properties

    @pytest.mark.skipif(not mixin_enabled, reason='')
    def test_viewmodel_derive_conflict_get_property(self):
        v = makeClass('ViewModel', 'Collide')()
        assert '1' not in v.properties
        v.setProperty('1', 2)
        assert v.getProperty('1') == 2

    # TODO:处理在ViewModel右边时。需要获取是否命名避让


if __name__ == '__main__':
    pass
