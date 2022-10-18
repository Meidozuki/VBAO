//
// Created by user on 2022/9/15.
//

#pragma once

#include "VBaoBase.hpp"

namespace vbao {

/**
 * @brief The most basic form of a MVVM model.
 */
class ModelBase {
public:
    PropertyNotifier prop_notice_; // Stores listener from ViewModel

    //参数为基类指针
    void addPropertyListener (PropertyNotificationVBase *p) {
        prop_notice_.addPropertyNotification(p);
    }
    void triggerPropertyNotification(const std::string &str) {
        prop_notice_.fireOnPropertyChanged(str);
    }

    virtual ~ModelBase() = default;
};
}
