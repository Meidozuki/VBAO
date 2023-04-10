from base import *


class Model:
    def __init__(self):
        self.prop_notice = PropertyNotifier()

        self.property = {}

    def addPropertyListener(self, listener: PropertyListenerBase):
        self.prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self.prop_notice.triggerPropertyNotifications(name)


class ViewModel:
    def __init__(self):
        self.prop_notice = PropertyNotifier()
        self.cmd_notice = CommandNotifier()
        self.listener = None
        self.model = None

        self.commands = {}
        self.property = {}

    def addPropertyListener(self, listener: PropertyListenerBase):
        self.prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self.prop_notice.triggerPropertyNotifications(name)

    def addCommandListener(self, listener: CommandListenerBase):
        self.cmd_notice.addNotification(listener)

    def triggerCommandNotifications(self, name: str, success: bool):
        self.cmd_notice.triggerCommandNotifications(name, success)

    def bindModel(self, model: Model):
        self.model = model

    def setListener(self, listener):
        self.listener = listener


class View:
    def __init__(self):
        self.prop_listener = None
        self.cmd_listener = None

        self.commands = {}
        self.property = {}


class App:
    def bind(self, model, viewmodel, view):
        # prop bindings
        model.property.update(viewmodel.property)
        view.property.update(viewmodel.property)

        # cmd bindings
        view.commands.update(viewmodel.commands)

        # event notification
        model.addPropertyListener(viewmodel.listener)
        viewmodel.addPropertyListener(view.prop_listener)
        viewmodel.addCommandListener(view.cmd_listener)
