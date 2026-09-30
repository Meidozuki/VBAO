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
from functools import wraps, partial

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


def _create_both_mixin(property_mixin: type, command_mixin: type):
    """
    If both prop&cmd are object, return object
    And throw AssertionError if only one is object
    Else, return a class derived from them two
    """
    if property_mixin is object or command_mixin is object:
        assert property_mixin is object and command_mixin is object
        return object
    else:
        class PropCmdMixinImpl(property_mixin, command_mixin):
            pass

        return PropCmdMixinImpl


_PropMixinType = Union[Type[PropertyMixinImpl], Type]
_CmdMixinType = Union[Type[CommandMixinImpl], Type]


def _create_mix_in_class() -> Tuple[_PropMixinType, _CmdMixinType]:
    """
    Dynamically build the PropertyMixin and CommandMixin classes based on the config,
    to avoid method name collisions with third-party libs like Qt.

    The return value depends on the flags in ConfigSingleton:
    - kNoMixin: mixins are disabled, return (object, object);
    - kOriginalMixin: return the original classes (PropertyMixinImpl, CommandMixinImpl);
    - otherwise: create new classes via type(), copying methods from the impl classes.
        If kAddSuffix is set, each method gets a "_vbao"-suffixed alias
    """
    from vbao.config import _config
    from vbao.config import ConfigOption as Options
    configs = _config.get()

    if Options.kNoMixin in configs:
        return object, object
    if Options.kOriginalMixin in configs:
        return PropertyMixinImpl, CommandMixinImpl
    # else, dynamically create the type


    def dict_op(d:dict, iterable, fn):
        for it in iterable:
            fn(d, it)

    def add_suffixed_attrs(d:dict, attr_name:str):
        if attr_name in d:
            d[attr_name + '_vbao'] = d[attr_name]

    def copy_attributes(template_class:type, out_dict:dict, attr:str):
        out_dict[attr] = getattr(template_class, attr)

    def create_property_mix_in(copy_attrs:Iterable[str]):
        nonlocal configs, avoid_func
        if Options.kOriginalMixin in configs:
            return PropertyMixinImpl

        def __init__(self, *args, **kwargs):
            super(PropertyMixin, self).__init__(*args, **kwargs)
            self._prop_mixin_helper = DictMixinHelper(self, 'properties')

        namespace = {"__init__": __init__}
        dict_op(namespace, copy_attrs, partial(copy_attributes, PropertyMixinImpl))

        if Options.kAddSuffix in configs:
            dict_op(namespace, avoid_func, add_suffixed_attrs)
        return type('PropertyMixin', (object,), namespace)

    def create_command_mix_in(copy_attrs:Iterable[str]):
        nonlocal configs, avoid_func
        if Options.kOriginalMixin in configs:
            return CommandMixinImpl

        def __init__(self, *args, **kwargs):
            super(CommandMixin, self).__init__(*args, **kwargs)
            self._cmd_mixin_helper = DictMixinHelper(self, 'commands')

        namespace = {"__init__": __init__}
        dict_op(namespace, copy_attrs, partial(copy_attributes, CommandMixinImpl))

        if Options.kAddSuffix in configs:
            dict_op(namespace, avoid_func, add_suffixed_attrs)
        return type('CommandMixin', (object,), namespace)

    # These tuples should be sort in dictionary order
    avoid_func = ("getProperty", "hasProperty", "setProperty",
                  "getCommand", "hasCommand", "setCommand")
    prop_copy_attrs = ("getProperty", "hasProperty", "setProperty")
    cmd_copy_attrs = ("getCommand", "hasCommand", "registerCommands", "runCommand", "setCommand")

    return create_property_mix_in(prop_copy_attrs), create_command_mix_in(cmd_copy_attrs)


PropertyMixin, CommandMixin = _create_mix_in_class()
PropertyCommandMixin = _create_both_mixin(PropertyMixin, CommandMixin)
