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

from abc import ABC
from functools import wraps
from typing import final

from .base import CommandBase, CommandListenerBase, PropertyListenerBase
from .core import ViewModel


@final
class DummyPropListener(PropertyListenerBase):
    # No need to take args, except for `to_whom` to keep compatible with normal listeners
    # Throws as soon as possible
    @wraps(PropertyListenerBase.__init__)
    def __init__(self, parent=None):
        super().__init__(parent)

    def onPropertyChanged(self, prop_name: str):
        raise NotImplementedError("You should not call on a DummyPropListener")


@final
class DummyCmdListener(CommandListenerBase):
    # No need to take args, except for `to_whom` to keep compatible with normal listeners
    # Throws as soon as possible
    @wraps(CommandListenerBase.__init__)
    def __init__(self, parent=None):
        super().__init__(parent)

    def onCommandComplete(self, cmd_name: str, success: bool):
        raise NotImplementedError("You should not call on a DummyCmdListener")


class CommandDirectCallMixin(CommandBase, ABC):
    """
    can call self.directCall() with parameter in 1 step
    Unfortunately, without override, in `self.execute` you still need to get the args by `self.args` and `self.kwargs`
    """

    def directCall(self, *args, **kwargs):
        self.setParameter(*args, **kwargs)
        self.execute()


class CommandBaseWithOwner(CommandBase, ABC):
    """
    __init__ receives an argument and stores it in self.owner.
    """

    def __init__(self, owner: ViewModel, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.owner: ViewModel = owner


class CommandBaseWithDict(CommandBase, ABC):
    """
    __init__ receives a dict and stores it in self.dict. Allows None.
    """

    def __init__(self, dict_, *args, **kwargs):
        super().__init__(*args, **kwargs)
        if dict_ is not None and not isinstance(dict_, dict):
            raise TypeError(f"CommandBaseWithDict() expects a dict argument but receives {type(dict_)}")
        self.dict = dict_


class LambdaCommand(CommandBase):
    """
    stores a function, and `self.execute` will call it with args passed by `self.setParameter`
    """

    def __init__(self, function, *args, **kwargs):
        super().__init__(*args, **kwargs)
        if not callable(function):
            raise TypeError(f"LambdaCommand() expects a function argument but receives {type(function)}")
        self.fn = function

    def execute(self):
        return self.fn(*self.args, **self.kwargs)
