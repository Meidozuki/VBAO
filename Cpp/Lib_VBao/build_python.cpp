
#include <iostream>

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include "VBAO/VBaoBase.hpp"
#include "VBAO/model.hpp"
using namespace vbao;

#include "python_wrapper.hpp"


int main(){
#ifdef BUILD_PYBIND
    std::cout << "build pybind\n";
#endif

}

#ifdef BUILD_PYBIND

PYBIND11_MODULE(vbao,m){
    py::object abc_base = py::module_::import("abc").attr("ABC");

    py::class_<CommandVBase, PyCommandBase>(m,"CommandBase",py::dynamic_attr())
        .def(py::init<>())
        .def("setParameter",&CommandVBase::setParameter)
        .def("execute",&CommandVBase::execute);

//    py::class_<PropertyNotificationVBase>(m,"PropertyListener")
//        .def("onPropertyChanged",&PropertyNotificationVBase::onPropertyChanged);
//
//    py::class_<CommandNotificationVBase>(m,"CommandListener")
//        .def("onCommandComplete",&CommandNotificationVBase::onCommandComplete);

    py::class_<PropertyNotifier>(m,"PropertyNotifier")
        .def("addPropertyNotification",&PropertyNotifier::addPropertyNotification)
        .def("removePropertyNotification",&PropertyNotifier::removePropertyNotification)
        .def("triggerOnPropertyChanged",&PropertyNotifier::fireOnPropertyChanged);

    py::class_<CommandNotifier>(m,"CommandNotifier")
        .def("addCommandNotification",&CommandNotifier::addCommandNotification)
        .def("removeCommandNotification",&CommandNotifier::removeCommandNotification)
        .def("triggerOnCommandChanged",&CommandNotifier::fireOnCommandComplete);

    // model
    py::class_<ModelBase>(m,"ModelBase")
        .def("triggerPropertyNotification",&ModelBase::triggerPropertyNotification)
        .def("_addPropertyListener",&ModelBase::addPropertyListener);


}


#endif