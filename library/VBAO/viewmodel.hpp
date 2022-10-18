//
// Created by user on 2022/9/16.
//

#pragma once

#include <memory>
#include <cassert>

#include "VBaoBase.hpp"
#include "listener.hpp"
#include "model.hpp"

class Window;
namespace vbao {

template<class ModelType = ModelBase>
class TViewModelBase {
    //---Property
public:
    using VMBaseType = TViewModelBase<ModelType>;
    // Stores listeners from Window
    PropertyNotifier prop_notice_;
    CommandNotifier cmd_notice_;
private:
    std::unique_ptr<ModelType> model_;
    std::unique_ptr<PropertyNotificationVBase> listener_;

    //---Method
public:
    TViewModelBase(): prop_notice_(), cmd_notice_(),
                      model_(nullptr), listener_(nullptr) {}

    // 添加来自Window的Listener
    void addPropertyListener(PropertyNotificationVBase *p) {
        prop_notice_.addPropertyNotification(p);
    }
    void addCommandListener(CommandNotificationVBase *p) {
        cmd_notice_.addCommandNotification(p);
    }
    // Viewmodel本身不trigger，而是listener在onXXX时触发
    void triggerPropertyNotification(const std::string &str) {
        prop_notice_.fireOnPropertyChanged(str);
    }
    void triggerCommandNotification(const std::string &str,bool bOK) {
        cmd_notice_.fireOnCommandComplete(str, bOK);
    }
    void bindModel(ModelBase *model) {
        model->addPropertyListener(listener_.get());
    }

protected:
    [[nodiscard]] ModelType* getModel() const {
        //TODO(weak):改善报错
        assert(model_.get() != nullptr);
        return model_.get();
    }

    /**
     * @brief Set the Model to the ViewModel, and bind (add a listener to) Model;
     * @param pModel: The pointer of the Model
     */
    //目前是protected，在有多个model时可能需要改成public
    void setModel(std::unique_ptr<ModelType> &&pModel) {
        //直接赋值，不需要cast，故使用move
        model_ = std::move(pModel);
        //addPropertyListener形参为基类指针
        bindModel(model_.get());
    }

    /**
     * @brief Set the listener of ViewModel. Must be called before setModel().
     * @param p (type std::unique_ptr<>&&)  The pointer of the Listener.
     */
    template<class DerivedListener>
    void setListener(std::unique_ptr<DerivedListener> &&p) {
        //此处需要cast为基类，但unique ptr不支持static ptr cast
        listener_ = std::unique_ptr<PropertyNotificationVBase>(p.release());
    }


};
}