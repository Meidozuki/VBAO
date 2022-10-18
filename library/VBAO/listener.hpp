//
// Created by user on 2022/9/13.
//

#pragma once

#include "VBaoBase.hpp"

namespace vbao{

/**
 * @brief Listener只做一件事，被执行时（数据改变或指令完成）通知所指向的观察者
 *      换言之Listener包装了Upper，提供给下层
 *      创建listener只是为了写viewmodel和window的sink时复用
 *      Listener的具体行为需要实现onPropertyChanged
 * @tparam Upper
 */
template <class Upper>
class PropertyListener : public PropertyNotificationVBase{
public:
    explicit PropertyListener(Upper *p): pNotify_(p) {}
protected:
    Upper *pNotify_; //使用裸指针，表示仅借用，没有所有权
};

/**
 * @brief 与PropertyListener类似，在指令之后触发通知上层
 *      Listener的具体行为需要实现onCommandComplete
 * @tparam Upper
 */
template <class Upper>
class CommandListener : public CommandNotificationVBase{
public:
  explicit CommandListener(Upper *p): pNotify_(p) {}
protected:
  Upper *pNotify_; //使用裸指针，表示仅借用，没有所有权
};

}

