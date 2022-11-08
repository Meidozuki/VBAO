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
    void onPropertyChanged(const std::string& prop_name) override = 0;
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
    void onCommandComplete(const std::string& cmd_name, bool bOK) override = 0;
protected:
    Upper *pNotify_; //使用裸指针，表示仅借用，没有所有权
};

}

