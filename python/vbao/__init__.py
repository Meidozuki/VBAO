import sys
import importlib.util


def lazy_import(name):
    spec = importlib.util.find_spec(name)
    loader = importlib.util.LazyLoader(spec.loader)
    spec.loader = loader
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    loader.exec_module(module)
    return module


from .base import CommandBase, CommandListenerBase, PropertyListenerBase
from .config import qt_installed, setConfig
from .convenient_class import *

core = lazy_import('vbao.core')
