import re
import logging
import importlib

import pytest
from test_util import vbao, reset, checkModuleInstalled

import vbao.config
from vbao.config import setConfig, useEasydict
import vbao.core.mixin as mix

easydict_installed = checkModuleInstalled('easydict')


def test_reset():
    reset()


class TestConfigBase:
    @classmethod
    def setup_class(cls):
        # run only once
        cls.config = vbao.config._config
        cls.debug_print_frozen = True

    def setup_method(self, method):
        # run before every method
        if self.debug_print_frozen:
            print(f"\n---setup--- config.frozen is {self.config.frozen}")
        reset()


class TestConfig(TestConfigBase):
    @classmethod
    def setup_class(cls):
        super().setup_class()
        cls.debug_print_frozen = False

    def reloadCore(self):
        # core.py will import DictCons, which will not change after importing.
        # In one-time run, the classes are always loaded after config.
        # But in unittest env, the core.py classes are fixed after the first time import,
        # so here we need to reload to get the newest change
        import vbao.core.core
        return importlib.reload(vbao.core.core)

    def assertConfigWarning(self, records):
        assert len(records) == 1
        assert records[0].levelno == logging.WARNING
        assert re.match(".*set config (before|after).*finalize it.?", records[0].msg)

    def test_config_freeze(self):
        assert vbao.config._config_frozen() is False
        self.config.get()
        assert vbao.config._config.frozen is True

    def test_config_frozen_warn(self, caplog):
        self.config.get()
        setConfig(original_mixin=True)

        self.assertConfigWarning(caplog.records)

    # If using from ... import DictCons, it seems like the changes of DictCons will not be synchronized
    def test_config_easydict_freeze(self, caplog):
        assert vbao.config.DictCons is dict
        self.config.get()
        useEasydict(False)
        assert vbao.config.DictCons is dict

        self.assertConfigWarning(caplog.records)

    def test_config_easydict_false(self):
        assert vbao.config.DictCons is dict
        useEasydict(False)
        assert vbao.config.DictCons is dict

    @pytest.mark.skipif(not easydict_installed, reason="easydict not installed")
    def test_config_easydict_true(self):
        from easydict import EasyDict
        assert vbao.config.DictCons is dict
        useEasydict(True)
        assert vbao.config.DictCons is EasyDict

    @pytest.mark.skipif(not easydict_installed, reason="easydict not installed")
    def test_config_easydict_construct(self):
        from easydict import EasyDict
        useEasydict(True)
        module = self.reloadCore()
        vm = module.ViewModel()
        assert isinstance(vm.properties, EasyDict)
        assert isinstance(vm.commands, EasyDict)


class TestMixin(TestConfigBase):
    @classmethod
    def setup_class(cls):
        super().setup_class()
        cls.debug_print_frozen = True

    def reloadMixin(self):
        # Like reloadCore above, PropertyMixin is fixed after the first run.
        # So we need to update it
        import vbao.core.mixin
        return importlib.reload(vbao.core.mixin)

    def test_mixin_original(self):
        obj = mix.PropertyMixinImpl()
        for fn in ("getProperty", "setProperty", "hasProperty"):
            assert hasattr(obj, fn)
            assert not hasattr(obj, fn + '_vbao')

    def test_mixin_add_suffix(self):
        setConfig(add_suffix=True)
        module = self.reloadMixin()
        obj = module.PropertyMixin()
        for fn in ("getProperty", "setProperty", "hasProperty"):
            assert hasattr(obj, fn)
            assert hasattr(obj, fn + '_vbao')
