//
// Created by user on 2022/10/13.
//

#include "viewmodel_sink.hpp"

#include "viewmodel.hpp"

ViewmodelListener::ViewmodelListener(Viewmodel *p) : PropertyListener(p)
{
}
void ViewmodelListener::onPropertyChanged(const std::string &prop_name) {
    //model层number变动后通知viewmodel
    auto &pVM = pNotify_;
    if (prop_name == "number") {
        *(pVM->pNumber_) = pVM->getModel()->getNumber();
        pVM->triggerPropertyNotification(prop_name);
    }

}
