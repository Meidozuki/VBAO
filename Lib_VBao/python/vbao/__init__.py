from .base import *

DictCons = dict


def use_easydict(b=True):
    global DictCons
    if b:
        from easydict import EasyDict
        DictCons = EasyDict
    else:
        DictCons = dict


class Model:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.prop_notice = PropertyNotifier()

        # self.property will conflict with Qt
        # TODO: make a setter function for checking, but self.setProperty conflicts with Qt
        self.properties = DictCons()

    def addPropertyListener(self, listener: PropertyListenerBase):
        self.prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self.prop_notice.triggerPropertyNotifications(name)


class ViewModel:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.prop_notice = PropertyNotifier()
        self.cmd_notice = CommandNotifier()
        self.listener = None
        self.model = None

        self.commands = DictCons()
        self.properties = DictCons()

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

    def runCommand(self, cmd_name: str):
        """
        Deprecated. Prefer to run command in View.
        """
        if cmd_name not in self.commands.keys():
            raise ValueError(f"Trying to call an invalid command {cmd_name}. "
                             f"Candidates are: {self.commands.keys()}")

        self.commands.get(cmd_name).execute()


class View:
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.prop_listener = None
        self.cmd_listener = None

        self.commands = DictCons()
        self.properties = DictCons()

    def runCommand(self, cmd_name: str):
        if cmd_name not in self.commands.keys():
            raise ValueError(f"Trying to call an invalid command {cmd_name}. "
                             f"Candidates are: {self.commands.keys()}")

        self.commands.get(cmd_name).execute()


class App:
    def bind(self, model, viewmodel, view, bind_vm_n_model=False):
        if bind_vm_n_model:
            viewmodel.bindModel(model)
        # prop bindings
        model.properties = viewmodel.properties
        view.properties = viewmodel.properties

        # cmd bindings
        view.commands = viewmodel.commands

        # event notification
        model.addPropertyListener(viewmodel.listener)
        viewmodel.addPropertyListener(view.prop_listener)
        viewmodel.addCommandListener(view.cmd_listener)
