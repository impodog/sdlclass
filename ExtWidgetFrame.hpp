//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETFRAME_HPP
#define SDLCLASS_TEST_EXTWIDGETFRAME_HPP

#include "ExtBase.h"
#include "ExtWidget.hpp"

NS_BEGIN
    WIDGET_TEMPLATE(=KeyMgr<>)
    class Frame : public WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        Surface surface;
    public:
        Frame(PointRef pos, SDLSurfacePtr surface, bool independent = true) : WidgetParent{pos},
                                                                              surface(surface, independent) {}

        void set_surface(SDLSurfacePtr new_surface, bool independent = true) {
            surface = {new_surface, independent};
        }

        WIDGET_DELETES(Frame)

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_frame);
        }

        WIDGET_PRESENT override {
            surface.copy_to(renderer, this->pos + rel);
        };

        WIDGET_TYPE(WidgetResult::t_frame)
    };
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETFRAME_HPP
