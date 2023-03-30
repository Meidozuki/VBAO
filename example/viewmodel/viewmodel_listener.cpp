//
// Created by user on 2022/10/13.
//

#include "viewmodel_listener.hpp"

#include "viewmodel.hpp"

ViewmodelListener::ViewmodelListener(Viewmodel *p) : PropertyListener(p)
{
}
void ViewmodelListener::onPropertyChanged(const std::string &prop_name) {
    //model层number变动后通知viewmodel
    auto &pVM = pNotify_;
    if (prop_name == "time") {
        *(pVM->pNumber_) = pVM->getModel()->getTimeNumber();
        pVM->triggerPropertyNotification(prop_name);
    }
    else if (prop_name == "sum") {
        //model不持有数据，不应该到此分支
    }

}
