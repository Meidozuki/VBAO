import logging
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
        self._prop_notice = PropertyNotifier()

        # self.property will conflict with Qt
        # None here means it should be set by App.bind()
        self.properties: Dict[str, Any] = None

    def addPropertyListener_from_vm(self, listener: PropertyListenerBase):
        # NotificationHolder can check validity, but we want to provide better error info
        if not isinstance(listener, PropertyListenerBase):
            raise TypeError(f"expect PropertyListenerBase, but get {type(listener)}")
        self._prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self._prop_notice.triggerPropertyNotifications(name)


class ViewModel(PropertyMixin, CommandMixin):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._prop_notice = PropertyNotifier()
        self._cmd_notice = CommandNotifier()
        self._prop_listener = None
        # Viewmodel contains Model, the result of running “commands” of Model can be directly seen
        self.model = None  # lots of operations will be done with Model, model is better than _model

        self.commands: Dict[str, CommandBase] = DictCons()
        self.properties: Dict[str, Any] = DictCons()

    def addPropertyListener_from_view(self, listener: PropertyListenerBase):
        if not isinstance(listener, PropertyListenerBase):
            raise TypeError(f"expect PropertyListenerBase, but get {type(listener)}")
        self._prop_notice.addNotification(listener)

    def triggerPropertyNotifications(self, name: str):
        self._prop_notice.triggerPropertyNotifications(name)

    def addCommandListener_from_view(self, listener: CommandListenerBase):
        if not isinstance(listener, CommandListenerBase):
            raise TypeError(f"expect CommandListenerBase, but get {type(listener)}")
        self._cmd_notice.addNotification(listener)

    def triggerCommandNotifications(self, name: str, success: bool):
        self._cmd_notice.triggerCommandNotifications(name, success)

    # extension
    def bindModel(self, model: Model, *, verbose=True):
        self.model = model
        model.properties = self.properties
        if self._prop_listener is None:
            if verbose:
                logging.warning("You are binding Model to VM without a listener, set verbose=True to suppress warning.")
        else:
            model.addPropertyListener_from_vm(self._prop_listener)

    @property
    def isModelSet(self):
        return self.model is not None

    def setListener(self, listener: PropertyListenerBase):
        self._prop_listener = listener

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
        # model.properties = viewmodel.properties # done in bindModel
        view.properties = viewmodel.properties

        # cmd bindings
        view.commands = viewmodel.commands

        # event notification
        # model.addPropertyListener_from_vm(viewmodel._prop_listener) # done in bindModel
        viewmodel.addPropertyListener_from_view(view.prop_listener)
        viewmodel.addCommandListener_from_view(view.cmd_listener)