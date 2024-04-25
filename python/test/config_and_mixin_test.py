import re
import logging
import importlib
from functools import wraps

import pytest
from test_util import vbao, reset

import vbao.core.mixin as mix
from vbao.config import setConfig


class TestMixin:
    @classmethod
    def setup_class(cls):
        # run only once
        cls.config = vbao.config._config
        cls.debug_print_frozen = True

    def setup_method(self, method):
        # run before every method
        if self.debug_print_frozen:
            print(f"\n---setup--- config.frozen is {self.config.frozen}, unlocking...")
        reset()  # This is important!
        setConfig(False,False,False,False)

    @wraps(vbao.setConfig)
    def setConfigAndReload(self, **kwargs):
        def reloadMixin():
            # Like reloadCore above, PropertyMixin is fixed after the first run.
            # So we need to update it
            import vbao.core.mixin
            return importlib.reload(vbao.core.mixin)

        setConfig(**kwargs)
        return reloadMixin()

    def test_info(self):
        print('\n', help(vbao.setConfig), sep='', end='')

    def test_prop_mixin_0(self):
        obj = mix.PropertyMixinImpl()
        for fn in ("getProperty", "hasProperty", "setProperty"):
            assert hasattr(obj, fn)
            assert not hasattr(obj, fn + '_vbao')

    def test_prop_mixin_use_original_class(self):
        module = self.setConfigAndReload(original_mixin=True)
        obj = module.PropertyMixin()
        for fn in ("getProperty", "hasProperty", "setProperty"):
            assert hasattr(obj, fn)
            assert not hasattr(obj, fn + '_vbao')

    def test_prop_mixin_add_suffix(self):
        module = self.setConfigAndReload(add_suffix=True)
        obj = module.PropertyMixin()
        for fn in ("getProperty", "hasProperty", "setProperty"):
            assert hasattr(obj, fn)
            assert hasattr(obj, fn + '_vbao')

    def test_cmd_mixin_0(self):
        obj = mix.CommandMixinImpl()
        for fn in ("getCommand", "hasCommand", "setCommand"):
            assert hasattr(obj, fn)
            assert not hasattr(obj, fn + '_vbao')

    def test_cmd_mixin_0_complete(self):
        obj = mix.CommandMixinImpl()
        for fn in ("getCommand", "hasCommand", "registerCommands", "runCommand", "setCommand"):
            assert hasattr(obj, fn)
            assert not hasattr(obj, fn + '_vbao')

    def test_cmd_mixin_use_original_class(self):
        module = self.setConfigAndReload(original_mixin=True)
        obj = module.CommandMixin()
        for fn in ("getCommand", "hasCommand", "setCommand"):
            assert hasattr(obj, fn)
            assert not hasattr(obj, fn + '_vbao')

    def test_cmd_mixin_add_suffix(self):
        module = self.setConfigAndReload(add_suffix=True)
        obj = module.CommandMixin()
        for fn in ("getCommand", "hasCommand", "setCommand"):
            assert hasattr(obj, fn)
            assert hasattr(obj, fn + '_vbao')

    def test_no_mixin_config(self):
        import vbao.core.core
        mixin = self.setConfigAndReload(no_mixin=True)
        assert mixin.PropertyMixin is object
        assert mixin.CommandMixin is object
        assert mixin.PropertyCommandMixin is object

        core = importlib.reload(vbao.core.core)
        # assert direct derive from object
        # if error, pytest will print mro() list
        for cls in (core.Model, core.ViewModel, core.View):
            assert len(cls.mro()) == 2
