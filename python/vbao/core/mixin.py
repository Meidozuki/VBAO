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

from vbao.config import _config, ConfigOption
from vbao.base import CommandBase


def addDoc(function):
    s = ('You may see a function with suffix _vbao, which is the same as this one, '
         'and is designed to avoid name collision with Qt, etc.')
    if function.__doc__ is None:
        function.__doc__ = s
    else:
        function.__doc__ += '\n' + s
    return function


# The avoidance acts like a metaclass
# But qt has its own metaclass (Shiboken.ObjectType) and will throw a TypeError
# So cannot use metaclass
# See https://docs.python.org/3/reference/datamodel.html#metaclasses


# It can be done via an if-statement in class definition
# But by doing so, type hint in PyCharm will be a mess
# So use subclasses to get a better type hint


class DictMixinHelper:
    """
    We use MixinHelper to reduce repeated codes
    But property and command both have their own target attribute
    So this is an aggregation, not a generalization
    """

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

    def getValue(self, key: str, verbose: bool = False) -> Optional[Any]:
        # if hit, needn't call
        if key in self._inner_dict:
            return self._inner_dict[key]
        else:
            if verbose:
                # already miss, just for the output
                self.find(key, True)
            return None


class PropertyMixinImpl:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._prop_mixin_helper = DictMixinHelper(self, 'properties')

    @addDoc
    def setProperty(self, key: str, value: Any) -> None:
        return self._prop_mixin_helper.emplace(key, value)

    @addDoc
    def getProperty(self, key: str, *args, **kwargs) -> Optional[Any]:
        return self._prop_mixin_helper.getValue(key, *args, **kwargs)

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
    def getCommand(self, key: str, *args, **kwargs) -> Optional[CommandBase]:
        return self._cmd_mixin_helper.getValue(key, *args, **kwargs)

    @addDoc
    def hasCommand(self, key: str, verbose: bool = True) -> bool:
        return self._cmd_mixin_helper.find(key, verbose)

    def runCommand(self, cmd_name: str):
        cmd = self.getCommand(cmd_name, verbose=True)
        if cmd:
            cmd.execute()

        self.commands.get(cmd_name).execute()

    def registerCommands(self, commands: Dict[str, Union[Type, CommandBase]]):
        """
        convenient function for register multiple commands.
        :param commands: dict. Key: str. Value: CommandBase subclass or instance.
        :return: a list contains invalid inputs
        """
        fail = []
        for key, cmd in commands.items():
            flag = True
            if isinstance(cmd, type):
                if issubclass(cmd, CommandBase):
                    cmd = cmd(self)
                else:
                    flag = False
            elif not isinstance(cmd, CommandBase):
                flag = False

            if flag:
                self.setCommand(key, cmd)
            else:
                print(f"registerCommands expect a CommandBase class or instance, but received {cmd} {type(cmd)}")
                fail.append((key, cmd))
        return fail


def _create_property_mix_in():
    from vbao.config import ConfigOption as Opt
    configs = _config.get()

    if Opt.kOriginalMixin in configs:
        return PropertyMixinImpl

    def __init__(self, *args, **kwargs):
        super(PropertyMixin, self).__init__(*args, **kwargs)
        self._prop_mixin_helper = DictMixinHelper(self, 'properties')

    attrs = {
        "__init__": __init__,
        "setProperty": PropertyMixinImpl.setProperty,
        "getProperty": PropertyMixinImpl.getProperty,
        "hasProperty": PropertyMixinImpl.hasProperty,
    }

    avoid_func = ("getProperty", "setProperty", "hasProperty", "getCommand", "setCommand")
    if Opt.kAddSuffix in configs:
        for ori_name in avoid_func:
            if ori_name in attrs:
                attrs[ori_name + '_vbao'] = attrs[ori_name]
    return type('PropertyMixin', (object,), attrs)


class PropertyMixinAvoidCollide(PropertyMixinImpl):
    setProperty_vbao = PropertyMixinImpl.setProperty
    getProperty_vbao = PropertyMixinImpl.getProperty
    hasProperty_vbao = PropertyMixinImpl.hasProperty


# if qt_installed:
#     CommandMixin = CommandMixinAvoidCollide
#     PropertyMixin = PropertyMixinAvoidCollide
# else:
#     CommandMixin = CommandMixinImpl
#     PropertyMixin = PropertyMixinImpl

CommandMixin = CommandMixinImpl
# PropertyMixin = PropertyMixinImpl
PropertyMixin = _create_property_mix_in()
