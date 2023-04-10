from abc import ABC, abstractmethod


class CommandBase(ABC):
    def setParameter(self, *args, **kwargs):
        self.args = args
        self.kwargs = kwargs

    @abstractmethod
    def execute(self):
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