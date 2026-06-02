// This file is part of VBAO.
//
// VBAO is free software: you can redistribute it and/or modify it under the terms of
// the GNU Lesser General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// VBAO is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with VBAO.
// If not, see <https://www.gnu.org/licenses/>.

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
