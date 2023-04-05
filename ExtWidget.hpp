//
// Created by Dogs-Cute on 4/1/2023.
//

#ifndef SDLCLASS_EXTWIDGET_HPP
#define SDLCLASS_EXTWIDGET_HPP

#include "ExtBase.h"
#include "ExtColor.hpp"
#include "ExtKeyMgr.hpp"

NS_BEGIN
    static const struct WidgetColorScheme {
        Color back_body, back_outline, front_body, front_outline;
    }
            scheme_dark{{25,  25,  25},
                        {70,  70,  70},
                        {50,  50,  50},
                        {145, 145, 145}},
            scheme_bright{{210, 220, 240},
                          {130, 130, 135},
                          {235, 240, 250},
                          {175, 180, 190}};

    class WidgetResult {
    protected:

        void free_results(bool same = false) {
            switch (type) {
                case t_none:
                case t_frame:
                    break;
                case t_button:
                    if (same)
                        result.button = {false, false};
                    break;
                case t_page:
                    if (same)
                        result.page->clear();
                    else
                        delete result.page;
                    break;
                case t_input_box:
                    if (same)
                        result.input_box.end = false;
                    break;
                case t_scrollbar:
                    if (same)
                        result.scrollbar.percentage = -1;
                    break;
                case t_branch_page:
                    if (same) {
                        result.branch_page->key = nullptr;
                        result.branch_page->page_res.clear();
                    } else
                        delete result.branch_page;
            }
        }

    public:
        enum WidgetType {
            t_none,
            t_button,
            t_page,
            t_input_box,
            t_scrollbar,
            t_branch_page,
            t_frame
        } type;
        union WidgetUnion {
            struct {
            } none;
            struct {
                bool pressed, released;
            } button;

            class PageResults {
            protected:
                /*Notice : Use all the void * as Widget Pointers*/
                using ConstWidgetPtr = const void *;
            public:
                std::unordered_map<ConstWidgetPtr, WidgetResult *> results;

                PageResults() = default;

                ~PageResults() {
                    for (auto &pair: results)
                        delete pair.second;
                }

                void insert(const std::pair<ConstWidgetPtr, WidgetResult *> &pair) {
                    results.insert(pair);
                }

                void clear() {
                    results.clear();
                }

                WidgetResult *&at(ConstWidgetPtr key) {
                    return results.at(key);
                }

                WidgetResult *operator[](ConstWidgetPtr key) noexcept {
                    return results[key];
                }
            } *page;

            struct {
                // This pointer always points to a basic_string for reference
                const void *str;
                bool end;
            } input_box;

            struct {
                long double percentage;
            } scrollbar;

            struct BranchPageResult {
                const void *key = nullptr;
                PageResults page_res;
            } *branch_page;

            struct {
            } frame;
        } result;

        WidgetResult() : type(t_none), result({.none={}}) {}

        explicit WidgetResult(WidgetType type) : type(type), result() {
            set_type(type);
        }

        WidgetResult(WidgetType type, WidgetUnion result) : type(type), result(result) {}

        ~WidgetResult() {
            free_results();
        }

        WidgetResult &operator=(const WidgetResult &) = delete;

        WidgetResult &operator=(WidgetResult &&) = delete;

        void set_type(WidgetType widgetType) {
            bool same = type == widgetType;
            free_results(same);
            type = widgetType;
            if (!same)
                switch (type) {
                    case t_none:
                        break;
                    case t_button:
                        result = {.button = {false, false}};
                        break;
                    case t_page:
                        result = {.page = new WidgetUnion::PageResults()};
                        break;
                    case t_input_box:
                        result = {.input_box = {nullptr, false}};
                        break;
                    case t_scrollbar:
                        result = {.scrollbar = {-1}};
                        break;
                    case t_branch_page:
                        result = {.branch_page = new WidgetUnion::BranchPageResult()};
                        break;
                    case t_frame:
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
        using WidgetParent = WIDGET_PARENT;
        using NumType = typeof(Point::x);
        using PointRef = typename Point::PointRef;
        using ConstSchemeRef = const WidgetColorScheme &;
        Point pos;

    public:
        explicit WidgetBase(const Point &pos) : pos(pos) {}

        virtual ~WidgetBase() = default;

        [[nodiscard]] constexpr const Point &view_pos() const noexcept {
            return pos;
        }

        WIDGET_DELETES(WidgetBase)

/* Processing the widget, called BEFORE present().
 Parameters:
 * rel : The relative processing position
 * mgr : The manager to listen to
 * result : Where processing results should be put at*/
        virtual WIDGET_PROCESS = 0;

/* Presenting the widget, called AFTER process().
 Parameters:
 * renderer : The renderer to copy to
 * rel : The relative presenting position*/
        virtual WIDGET_PRESENT = 0;


        [[nodiscard]] virtual constexpr WidgetResult::WidgetType get_type() const noexcept = 0;
    };
NS_END

#endif //SDLCLASS_EXTWIDGET_HPP
