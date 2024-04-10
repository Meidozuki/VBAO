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

from abc import ABC, abstractmethod


class CommandBase(ABC):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        # predefine None. For convenience of judging whether set parameter
        self.args = None
        self.kwargs = None

    def setParameter(self, *args, **kwargs):
        """
        Set parameters in self.args & self.kwargs
        """
        self.args = args
        self.kwargs = kwargs

    @abstractmethod
    def execute(self):
        """
        Prefer call onCommandComplete(self, cmd_name: str, success: bool), other than return bool
        """
        pass


class PropertyListenerBase(ABC):
    def __init__(self, to_whom):
        self.master = to_whom

    @abstractmethod
    def onPropertyChanged(self, prop_name: str):
        pass


class CommandListenerBase(ABC):
    def __init__(self, to_whom):
        self.master = to_whom

    @abstractmethod
    def onCommandComplete(self, cmd_name: str, success: bool):
        pass


class NotificationHolder:
    def __init__(self):
        self.arr = []

    def addNotification(self, input):
        if input is None:
            return
        elif isinstance(input, (PropertyListenerBase, CommandListenerBase)):
            self.arr.append(input)
        else:
            print("Not invalid type",input)

    def removeNotification(self, x):
        if isinstance(input, (PropertyListenerBase, CommandListenerBase)):
            self.arr.remove(x)

    def clear(self):
        self.arr.clear()


class PropertyNotifier(NotificationHolder):
    def triggerPropertyNotifications(self, name):
        for listener in self.arr:
            listener.onPropertyChanged(name)


class CommandNotifier(NotificationHolder):
    def triggerCommandNotifications(self, name, success):
        for listener in self.arr:
            listener.onCommandComplete(name, success)