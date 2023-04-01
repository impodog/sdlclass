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
            none,
            button
        } type;
        union WidgetUnion {
            struct {
            } none;
            struct {
                bool down, up;
            } button;
        } result;

        WidgetResult() : type(none), result({.none={}}) {}

        WidgetResult(WidgetType widgetType, WidgetUnion result) : type(widgetType), result(result) {}

        void set(WidgetType widgetType) {
            type = widgetType;
            switch (type) {
                case none:
                    break;
                case button:
                    result = {.button = {false, false}};
                    break;
            }
        }
    };

    WIDGET_TEMPLATE()
    class WidgetBase {
    protected:
        Point pos;

    public:
        explicit WidgetBase(const Point &pos) : pos(pos) {}

        /* Processing the widget, called BEFORE present().
         * rel : the relative processing position
         * mgr : the manager to listen to*/
        virtual void process(const Point &rel, const MgrType *mgr, WidgetResult &result) = 0;

        /* Presenting the widget, called AFTER process().
         * rel : the relative presenting position*/
        virtual void present(RendererPtr renderer, const Point &rel) = 0;
    };
NS_END

#endif //SDLCLASS_EXTWIDGET_HPP
