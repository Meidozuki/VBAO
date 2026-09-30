import logging
import re

import unittest
import pytest

from test_util import vbao
from vbao.notifier import PropertyNotifier, CommandNotifier
from vbao.core import Model, ViewModel, View, App


class TestEnv(unittest.TestCase):
    from unittest.mock import patch

    def test_error(self):
        with self.assertRaises(TypeError):
            raise TypeError

    def test_warning(self):
        with self.assertLogs('root', logging.WARNING) as cm:
            logging.warning('abc')

        self.assertEqual(cm.output, ['WARNING:root:abc'])
        self.assertEqual(len(cm.records), 1)
        self.assertEqual(cm.records[0].msg, 'abc')

    # @patch('logging.warning')
    # def test_vm_bind_model_no_listener(self, mocked):
    #     model = vbao.Model()
    #     viewmodel = vbao.ViewModel()
    #     viewmodel.bindModel(model, verbose=True)
    #     self.assertLogs('root', logging.WARNING)
    #     self.assertTrue(mocked.called)


def test_namespace():
    """
    顶层包 vbao 不应暴露核心类 Model/ViewModel/View。
    需要通过vbao.core访问，避免 __init__.py 污染命名空间。
    """
    with pytest.raises(AttributeError):
        vbao.Model()
    with pytest.raises(AttributeError):
        vbao.ViewModel()
    with pytest.raises(AttributeError):
        vbao.View()


class TestVBAOConstructor:
    def test_command(self):
        with pytest.raises(TypeError, match="^Can't instantiate abstract class"):
            vbao.CommandBase()

    def test_prop_listener(self):
        with pytest.raises(TypeError, match="^Can't instantiate abstract class"):
            vbao.PropertyListenerBase(None)

    def test_cmd_listener(self):
        with pytest.raises(TypeError, match="^Can't instantiate abstract class"):
            vbao.CommandListenerBase(None)

    def test_model(self):
        model = Model()

        assert model.properties is None

        assert hasattr(model, '_prop_notice')
        assert isinstance(model._prop_notice, PropertyNotifier)

        assert not hasattr(model, '_cmd_notice')

    def test_viewmodel(self):
        viewmodel = ViewModel()

        assert viewmodel.model is None
        assert viewmodel.commands == {}
        assert viewmodel.properties == {}
        assert viewmodel.isModelSet is False

        assert hasattr(viewmodel, '_prop_notice')
        assert isinstance(viewmodel._prop_notice, PropertyNotifier)
        assert hasattr(viewmodel, '_cmd_notice')
        assert isinstance(viewmodel._cmd_notice, CommandNotifier)

        assert viewmodel._prop_listener is None

    def test_view(self):
        view = View()

        assert view.commands is None
        assert view.properties is None

        assert view.prop_listener is None
        assert view.cmd_listener is None

        assert not hasattr(view, 'viewmodel')
        assert not hasattr(view, '_prop_notice')
        assert not hasattr(view, '_cmd_notice')


class TestVBAOAppSmoke:
    @staticmethod
    def _make():
        model, viewmodel, view = Model(), ViewModel(), View()
        view.prop_listener = vbao.DummyPropListener(view)
        view.cmd_listener = vbao.DummyCmdListener(view)
        return model, viewmodel, view

    def test_bind_default_flags(self):
        model, viewmodel, view = self._make()

        App.bind(model, viewmodel, view, False)

        assert view.properties is viewmodel.properties
        assert view.commands is viewmodel.commands

        assert viewmodel.isModelSet is False
        assert model.properties is None

        assert not hasattr(view, 'viewmodel')

    def test_bind_vm_n_model_and_debug(self):
        model, viewmodel, view = self._make()

        App.bind(model, viewmodel, view, True, debug_set_vm_in_view=True)

        assert viewmodel.isModelSet is True
        assert model.properties is viewmodel.properties

        assert view.viewmodel is viewmodel

    def test_dummy_listeners_are_reachable(self):
        model, viewmodel, view = self._make()

        App.bind(model, viewmodel, view)

        with pytest.raises(NotImplementedError):
            viewmodel.triggerPropertyNotifications('cnt')
        with pytest.raises(NotImplementedError):
            viewmodel.triggerCommandNotifications('inc', True)


class TempPropListener(vbao.PropertyListenerBase):
    def onPropertyChanged(self, prop_name: str):
        self.master.property["last_prop"] = prop_name


class TestVBAOCore:
    def test_vm_bind_model_no_listener(self, caplog):
        model = Model()
        viewmodel = ViewModel()
        viewmodel.bindModel(model, verbose=True)

        records = caplog.records
        assert len(records) == 1
        assert records[0].levelno == logging.WARNING
        assert re.search('binding Model to VM', caplog.text)

    def _setup_basic_model_vm(self):
        self.model = Model()
        self.viewmodel = ViewModel()
        self.viewmodel.setListener(TempPropListener(self.viewmodel))
        self.viewmodel.bindModel(self.model, verbose=True)

    def test_vm_bind_model_with_listener(self, caplog):
        self._setup_basic_model_vm()

        assert len(caplog.records) == 0


if __name__ == '__main__':
    pass
