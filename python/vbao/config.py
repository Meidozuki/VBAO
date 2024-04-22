# This file is part of VBAO.
#
# VBAO is free software: you can redistribute it and/or modify it under the terms of
# the GNU Lesser General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# VBAO is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with VBAO.
# If not, see <https://www.gnu.org/licenses/>.


import enum
import logging
from typing import Dict, FrozenSet

# ------ detect Qt ------
qt_installed = None
# find out if Qt is installed because Qt's property will conflict with VBAO
try:
    from PIL import ImageQt

    qt_installed = ImageQt.qt_is_installed
except ImportError:
    pass

if qt_installed is None:
    # PIL uninstalled, manually try import Qt
    qt_installed = False
    for qt_module in ["PySide6", "PyQt6", "PySide2", "PySide2"]:
        try:
            if qt_module == "PySide6":
                import PySide6
            elif qt_module == "PyQt6":
                import PyQt6
            elif qt_module == "PySide2":
                import PySide2
            elif qt_module == "PyQt5":
                import PyQt5
        except ImportError:
            continue

        qt_installed = True
        break


# ------

# ------ VBAO options ------


@enum.unique
class ConfigOption(enum.Enum):
    # google C++ enum/const style
    from enum import auto
    kNotSet = 0

    kNoMixin = auto()
    kOriginalMixin = auto()

    kAddSuffix = auto()
    kDropOriginal = auto()


class _ConfigSingleton:
    def __init__(self):
        self.frozen = False
        self.ret = None

        self._dict: Dict[str, bool] = {
            'original_mixin': False,
            'add_suffix': True,
            'suffix_and_delete_original': False,
        }

    def checkFrozen(self):
        if self.frozen:
            logging.warning("You should set config before any function finalize it")
        return self.frozen

    def set(self, key: str, value: bool):
        if self.checkFrozen():
            return
        if key not in self._dict:
            raise KeyError(f"vbao config does not take argument {key}")

        self._dict[key] = value

    def get(self) -> FrozenSet[ConfigOption]:
        if not self.frozen:
            self.frozen = True
            print('vbao config is freezing with', self._dict)

            res = []
            if self._dict['original_mixin']:
                res.append(ConfigOption.kOriginalMixin)
            elif self._dict['add_suffix']:
                res.append(ConfigOption.kAddSuffix)

            if len(res) == 0:
                res = (ConfigOption.kOriginalMixin,)

            self.ret = frozenset(res)
        return self.ret


_config = _ConfigSingleton()


def _config_frozen():
    return _config.checkFrozen()


def setConfig(original_mixin=None,
              add_suffix=None,
              suffix_and_delete_original=None):
    def wrap(**kwargs):
        for k, v in kwargs.items():
            if v is not None:
                _config.set(k, v)

    wrap(original_mixin=original_mixin,
         add_suffix=add_suffix,
         suffix_and_delete_original=suffix_and_delete_original)


# ------ for vbao.core ------

DictCons = dict


def use_easydict(use: bool = True):
    global DictCons
    # DictCons will be used by Model, etc. And they will lock the config, so now do not explicitly lock
    if _config_frozen():
        return

    if use:
        from easydict import EasyDict
        DictCons = EasyDict
    else:
        DictCons = dict
# ------
