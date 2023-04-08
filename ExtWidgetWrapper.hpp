//
// Created by Dogs-Cute on 4/8/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETWRAPPER_HPP
#define SDLCLASS_TEST_EXTWIDGETWRAPPER_HPP


#include "ExtBase.h"
#include "ExtWidget.hpp"

NS_BEGIN
#if __cplusplus >= 202002L
    template<typename WrapType>
    concept SupportsCopyTo = requires {
        WrapType().copy_to(RendererPtr(), Point());
    };

    WIDGET_TEMPLATE(, SupportsCopyTo WrapType)
#else
    WIDGET_TEMPLATE(, typename WrapType)
#endif
// This simply wraps anything that has member function copy_to(RendererPtr, const Point &)
    class Wrapper final : public WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        WrapType &obj;
    public:
        explicit Wrapper(PointRef pos, WrapType &obj) : WidgetParent{pos}, obj(obj) {}

        WIDGET_DELETES(Wrapper)

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_wrapper);
        }

        WIDGET_PRESENT override {
            obj.copy_to(renderer, this->pos + rel);
        };

        WIDGET_TYPE(WidgetResult::t_wrapper)
    };
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETWRAPPER_HPP
