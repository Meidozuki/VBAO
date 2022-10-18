//
// Created by user on 2022/9/15.
//

// project name 维包 viewmodel
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

class PropertyNotificationVBase {
public:
    virtual void onPropertyChanged(const std::string& prop_name) = 0;
};

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
