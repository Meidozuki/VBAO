//
// Created by user on 2022/10/13.
//

#ifndef LIBTEST_VIEWMODEL_VIEWMODEL_LISTENER_HPP_
#define LIBTEST_VIEWMODEL_VIEWMODEL_LISTENER_HPP_

#include "VBAO/listener.hpp"

class Viewmodel;

class ViewmodelListener : public vbao::PropertyListener<Viewmodel> {
public:
    explicit ViewmodelListener(Viewmodel *p);

    void onPropertyChanged(const std::string& prop_name) override;
};

#endif //LIBTEST_VIEWMODEL_VIEWMODEL_LISTENER_HPP_
