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

from typing import *
from functools import wraps

from .config import qt_installed
from .base import CommandBase


# avoid_qt = True
# keep_old_alias = False

def addDoc(function):
    s = ('You may see a function with suffix _vbao, which is the same as this one, '
         'and is designed to avoid name collision with Qt, etc.')
    if function.__doc__ is None:
        function.__doc__ = s
    else:
        function.__doc__ += '\n' + s
    return function


# It can be done via an if-statement in class definition
# But by doing so, type hint in PyCharm will be a mess
# So use subclasses to get a better type hint
class DictMixinHelper:
    def __init__(self, obj, attribute: str):
        assert isinstance(attribute, str)
        self.attribute = attribute
        self.obj = obj

    @property
    def _inner_dict(self) -> dict:
        return getattr(self.obj, self.attribute)

    def emplace(self, key: str, value: Any) -> None:
        self._inner_dict[key] = value

    def find(self, key: str, verbose: bool = True) -> bool:
        if key in self._inner_dict:
            return True
        elif verbose:
            print(f'{key} is not a valid {self.attribute}, candidates are {self._inner_dict.keys()}')
            return False

    def getValue(self, key: str) -> Optional[Any]:
        if key in self._inner_dict:
            return self._inner_dict[key]
        else:
            return None


class PropertyMixinImpl:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._prop_mixin_helper = DictMixinHelper(self, 'properties')

    @addDoc
    def setProperty(self, key: str, value: Any) -> None:
        return self._prop_mixin_helper.emplace(key, value)

    @addDoc
    def getProperty(self, key: str) -> Optional[Any]:
        return self._prop_mixin_helper.getValue(key)

    @addDoc
    def hasProperty(self, key: str, verbose: bool = True) -> bool:
        return self._prop_mixin_helper.find(key, verbose)


class CommandMixinImpl:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._cmd_mixin_helper = DictMixinHelper(self, 'commands')

    # change type annotation
    @addDoc
    def setCommand(self, key: str, value: CommandBase) -> None:
        self._cmd_mixin_helper.emplace(key, value)

    @addDoc
    def getCommand(self, key: str) -> Optional[CommandBase]:
        return self._cmd_mixin_helper.getValue(key)

    @addDoc
    def hasCommand(self, key: str, verbose: bool = True) -> bool:
        return self._cmd_mixin_helper.find(key, verbose)


class PropertyMixinAvoidCollide(PropertyMixinImpl):
    setProperty_vbao = PropertyMixinImpl.setProperty
    getProperty_vbao = PropertyMixinImpl.getProperty
    hasProperty_vbao = PropertyMixinImpl.hasProperty


class CommandMixinAvoidCollide(CommandMixinImpl):
    setCommand_vbao = CommandMixinImpl.setCommand
    getCommand_vbao = CommandMixinImpl.getCommand
    hasCommand_vbao = CommandMixinImpl.hasCommand


# if qt_installed:
#     CommandMixin = CommandMixinAvoidCollide
#     PropertyMixin = PropertyMixinAvoidCollide
# else:
#     CommandMixin = CommandMixinImpl
#     PropertyMixin = PropertyMixinImpl

CommandMixin = CommandMixinAvoidCollide
PropertyMixin = PropertyMixinAvoidCollide
