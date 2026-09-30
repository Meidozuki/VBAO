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

from .interfaces import CommandListenerBase, PropertyListenerBase


class NotificationHolder:
    def __init__(self):
        self.arr = []

    def addNotification(self, input):
        if input is None:
            return
        elif isinstance(input, (PropertyListenerBase, CommandListenerBase)):
            self.arr.append(input)
        else:
            raise TypeError(f"expect PropertyListenerBase or CommandListenerBase, but get {type(input)}")

    def removeNotification(self, x):
        if isinstance(x, (PropertyListenerBase, CommandListenerBase)):
            if x in self.arr:
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
