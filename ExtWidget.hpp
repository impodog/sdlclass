//
// Created by Dogs-Cute on 4/1/2023.
//

#ifndef SDLCLASS_EXTWIDGET_HPP
#define SDLCLASS_EXTWIDGET_HPP

#include "ExtBase.h"
#include "ExtColor.hpp"
#include "ExtKeyMgr.hpp"

NS_BEGIN
    class WidgetResult {
    public:
        enum WidgetType {
            t_none,
            t_button
        } type;
        union WidgetUnion {
            struct {
            } none;
            struct {
                bool pressed, released;
            } button;
        } result;

        WidgetResult() : type(t_none), result({.none={}}) {}

        WidgetResult(WidgetType widgetType, WidgetUnion result) : type(widgetType), result(result) {}

        void set(WidgetType widgetType) {
            type = widgetType;
            switch (type) {
                case t_none:
                    break;
                case t_button:
                    result = {.button = {false, false}};
                    break;
            }
        }
    };

/*The base class for all widgets
 * This contains one member variable : this->pos which defines the widget's position kept by itself.
 However, the widget's display position depends on both this->pos and the parameter "rel".
 * Thw widget is used by calling this->process() and this->present()*/
    WIDGET_TEMPLATE()
    class WidgetBase {
    protected:
        Point pos;
    public:
        explicit WidgetBase(const Point &pos) : pos(pos) {}

/* Processing the widget, called BEFORE present().
 Parameters:
 * rel : The relative processing position
 * mgr : The manager to listen to
 * result : Where processing results should be put at*/
        virtual void process(const Point &rel, const MgrType &mgr, WidgetResult &result) = 0;

/* Presenting the widget, called AFTER process().
 Parameters:
 * renderer : The renderer to copy to
 * rel : The relative presenting position*/
        virtual void present(RendererPtr renderer, const Point &rel) = 0;
    };
NS_END

#endif //SDLCLASS_EXTWIDGET_HPP
