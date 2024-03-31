from typing import *

from .base import *
from .mixin import PropertyMixin, CommandMixin

DictCons = dict


def use_easydict(b=True):
    global DictCons
    if b:
        from easydict import EasyDict
        DictCons = EasyDict
    else:
        DictCons = dict


class Model(PropertyMixin):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.prop_notice = PropertyNotifier()

        # self.property will conflict with Qt
        self.properties: Dict[str, Any] = None

    def addPropertyListener_from_vm(self, listener: PropertyListenerBase):
        self.prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self.prop_notice.triggerPropertyNotifications(name)


class ViewModel(PropertyMixin, CommandMixin):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.prop_notice = PropertyNotifier()
        self.cmd_notice = CommandNotifier()
        self.prop_listener = None
        self.model = None

        self.commands: Dict[str, CommandBase] = DictCons()
        self.properties: Dict[str, Any] = DictCons()

    def addPropertyListener_from_view(self, listener: PropertyListenerBase):
        self.prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self.prop_notice.triggerPropertyNotifications(name)

    def addCommandListener_from_view(self, listener: CommandListenerBase):
        self.cmd_notice.addNotification(listener)

    def triggerCommandNotifications(self, name: str, success: bool):
        self.cmd_notice.triggerCommandNotifications(name, success)

    def bindModel(self, model: Model):
        self.model = model

    def setListener(self, listener: PropertyListenerBase):
        self.prop_listener = listener

    def runCommand(self, cmd_name: str):
        """
        Deprecated. Prefer to run command in View.
        """
        if cmd_name not in self.commands.keys():
            raise ValueError(f"Trying to call an invalid command {cmd_name}. "
                             f"Candidates are: {self.commands.keys()}")

        self.commands.get(cmd_name).execute()


class View(PropertyMixin, CommandMixin):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.prop_listener = None
        self.cmd_listener = None

        # None here means it should be set by App.bind()
        self.commands: Dict[str, CommandBase] = None
        self.properties: Dict[str, Any] = None

    def runCommand(self, cmd_name: str):
        if cmd_name not in self.commands.keys():
            raise ValueError(f"Trying to call an invalid command {cmd_name}. "
                             f"Candidates are: {self.commands.keys()}")

        self.commands.get(cmd_name).execute()


class App:
    @classmethod
    def bind(cls, model: Model, viewmodel: ViewModel, view: View,
             bind_vm_n_model=False, *, debug_set_vm_in_view=False):
        if bind_vm_n_model:
            viewmodel.bindModel(model)
        if debug_set_vm_in_view:
            view.viewmodel = viewmodel
        # prop bindings
        model.properties = viewmodel.properties
        view.properties = viewmodel.properties

        # cmd bindings
        view.commands = viewmodel.commands

        # event notification
        model.addPropertyListener_from_vm(viewmodel.prop_listener)
        viewmodel.addPropertyListener_from_view(view.prop_listener)
        viewmodel.addCommandListener_from_view(view.cmd_listener)
