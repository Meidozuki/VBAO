import os, sys
import pytest
import importlib
import importlib.util

sys.path.insert(0, os.path.abspath('.'))
sys.path.insert(0, os.path.abspath('..'))
import vbao


def checkModuleInstalled(module_name) -> bool:
    spec = importlib.util.find_spec(module_name)
    if spec is not None:
        try:
            importlib.import_module(module_name)
        except ImportError:
            return False
        else:
            return True
    return False


def reset():
    # The config is frozen after get().
    # In test cases, we will run set&get multi times, and config will not be unlocked automatically.
    # So we need to manually unlock the config
    with pytest.deprecated_call():
        vbao.config._config._debug_reset()
