import logging
import re

import unittest
from unittest.mock import patch
import pytest

from test_util import vbao


class TestEnv(unittest.TestCase):
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
        model = vbao.Model()

    def test_viewmodel(self):
        viewmodel = vbao.ViewModel()

    def test_view(self):
        view = vbao.View()


class TempPropListener(vbao.PropertyListenerBase):
    def onPropertyChanged(self, prop_name: str):
        self.master.property["last_prop"] = prop_name


class TestVBAOCore:
    @classmethod
    def setup_class(cls):
        pass

    def test_vm_bind_model_no_listener(self, caplog):
        model = vbao.Model()
        viewmodel = vbao.ViewModel()
        viewmodel.bindModel(model, verbose=True)

        records = caplog.records
        assert len(records) == 1
        assert records[0].levelno == logging.WARNING
        assert re.search('binding Model to VM', caplog.text)

    def _setup_basic_model_vm(self):
        self.model = vbao.Model()
        self.viewmodel = vbao.ViewModel()
        self.viewmodel.setListener(TempPropListener(self.viewmodel))
        self.viewmodel.bindModel(self.model, verbose=True)

    def test_vm_bind_model_with_listener(self, caplog):
        self._setup_basic_model_vm()

        assert len(caplog.records) == 0


if __name__ == '__main__':
    pass
