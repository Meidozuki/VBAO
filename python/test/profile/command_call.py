import os, sys
import math
import time
import cProfile as profile
from pstats import SortKey
from PySide6.QtCore import QObject, Signal, Slot

# sys.path.append(os.path.abspath('..'))

import vbao

vbao.setConfig(no_mixin=True)
assert len(vbao.core.ViewModel.mro()) == 2

from vbao.core import Model, ViewModel, View  # 不这样的话不会自动补全


def model_action(inp):
    inp = inp * 2 + 1
    if inp > 1000:
        inp = math.floor(math.log2(inp))
    return inp


class DirectWhole:
    class Model:
        def __init__(self):
            self.view = None
            self.cnt = 0

        def inc(self):
            self.cnt = model_action(self.cnt)

        def callToView(self):
            self.view.cnt = self.cnt

    class View:
        def __init__(self):
            self.model = None
            self.cnt = 0

        def show(self):
            return self.cnt

        def callToModel(self):
            self.model.inc()

    def __init__(self):
        self.model = self.Model()
        self.view = self.View()
        self.model.view = self.view
        self.view.model = self.model

    def call(self):
        self.view.callToModel()
        self.model.callToView()
        self.view.show()


class VBaoWhole:
    class ViewModel(ViewModel):
        class Model(Model):
            def inc(self):
                self.properties["cnt"] = model_action(self.properties["cnt"])
                self.triggerPropertyNotifications("cnt")

        class VMPropListener(vbao.PropertyListenerBase):
            def onPropertyChanged(self, prop_name: str):
                self.master.triggerPropertyNotifications("cnt")

        class Command(vbao.CommandBaseWithOwner):
            def execute(self):
                self.owner.f()

        def __init__(self):
            super().__init__()
            self.properties["cnt"] = 0
            self.commands["f"] = self.Command(self)
            self.setListener(self.VMPropListener(self))

            model = self.Model()
            self.bindModel(model)

        def f(self):
            self.model.inc()

    class View(View):
        class PropListener(vbao.PropertyListenerBase):
            def onPropertyChanged(self, prop_name: str):
                self.master.show()

        def __init__(self):
            super().__init__()
            self.prop_listener = self.PropListener(self)
            self.cmd_listener = vbao.DummyCmdListener(self)

        def show(self):
            return self.properties["cnt"]

    def __init__(self):
        self.vm = self.ViewModel()
        self.view = self.View()
        vbao.core.App.bind(None, self.vm, self.view, bind_vm_n_model=False)

    def call(self):
        self.view.commands["f"].execute()


class QtWhole:
    class Model(QObject):
        def __init__(self):
            super().__init__()
            self.setProperty("cnt", 0)

        @Slot()
        def inc(self):
            self.setProperty("cnt", model_action(self.property("cnt")))

    class View(QObject):
        signal = Signal()
        def __init__(self, model):
            super().__init__()
            self.model = model

    def __init__(self):
        self.model = self.Model()
        self.view = self.View(self.model)
        self.view.signal.connect(self.model.inc)

    def call(self):
        self.view.signal.emit()



if __name__ == '__main__':
    direct = DirectWhole()
    qt = QtWhole()
    mvvm = VBaoWhole()

    test_times = 50000


    def f1():
        for _ in range(test_times):
            direct.call()
    def f2():
        for _ in range(test_times):
            mvvm.call()
    def f3():
        for _ in range(test_times):
            qt.call()


    direct.call()
    time.sleep(1)
    profile.run("f1()", sort=SortKey.TIME)

    mvvm.call()
    time.sleep(1)
    profile.run("f2()", sort=SortKey.TIME)

    mvvm.call()
    time.sleep(1)
    profile.run("f3()", sort=SortKey.TIME)