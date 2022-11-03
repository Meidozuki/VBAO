// This file is part of VBAO.
//
// VBAO is free software: you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// VBAO is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with VBAO.
// If not, see <https://www.gnu.org/licenses/>.

// project name 维包
#pragma once

#if __cplusplus < 201700L
#error This library needs a C++ version greater than C++17.
#endif

#include <string>
#include <vector>
#include <any>
#include <algorithm>
#include <cassert>

namespace vbao {

//---抽象基类，会将子类cast为Base类实现多态

/**
 * @brief 指令抽象基类，需要实现setParameter和execute
 */
class CommandVBase {
public:
    virtual void setParameter(const std::any &param) = 0;
    virtual void execute() = 0;
};

/**
 * @brief 属性变动通知抽象基类，需要实现onPropertyChanged
 */
class PropertyNotificationVBase {
public:
    virtual void onPropertyChanged(const std::string& prop_name) = 0;
};

/**
 * @brief 属性变动通知抽象基类，需要实现onCommandComplete
 */
class CommandNotificationVBase{
public:
    virtual void onCommandComplete(const std::string& cmd_name, bool bOK) = 0;
};

//---通知接口类
template<class T>
class NotificationHolder {
    //静多态
protected:
    std::vector<T*> array_;

    void clear() noexcept { array_.clear();}
    void addNotification(T *p) {array_.push_back(p);}
    bool removeNotification(T* const p) {
        auto iter = std::find(array_.begin(), array_.end(), p);
        bool flag = (iter != array_.end());
        if (flag) { array_.erase(iter);}
        return flag;
    }
};


class PropertyNotifier : public NotificationHolder<PropertyNotificationVBase>
{
public:
    void addPropertyNotification(PropertyNotificationVBase *p) {
        addNotification(p);
    }
    void removePropertyNotification(PropertyNotificationVBase *p) {
        removeNotification(p);
    }
    void fireOnPropertyChanged(const std::string& str) {
        for(auto &p: array_) {
            assert(p != nullptr);
            p->onPropertyChanged(str);
        }
    }
};


class CommandNotifier : public NotificationHolder<CommandNotificationVBase>
{
public:
    void addCommandNotification(CommandNotificationVBase *p){
        addNotification(p);
    }
    void removeCommandNotification(CommandNotificationVBase *p) noexcept{
        removeNotification(p);
    }
    void fireOnCommandComplete(const std::string& str, bool bOK){
        for(auto &p: array_) {
            p->onCommandComplete(str, bOK);
        }
    }
};

}
