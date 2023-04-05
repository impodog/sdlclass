//
// Created by Dogs-Cute on 4/4/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETPAGE_HPP
#define SDLCLASS_TEST_EXTWIDGETPAGE_HPP

#include "ExtBase.h"
#include "ExtWidget.hpp"

NS_BEGIN
/*A Page of contents that can manage every component repeatedly*/
    template<typename MgrType>
    class Page : WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        using ElementType = WidgetBase<MgrType>;
        std::unordered_map<ElementType *, WidgetResult::WidgetType> elements;
        bool independent;
    public:
        /*Parameters:
         * elements : a vector of Widgets with the same manager to be put on this page
         * independent : whether the page will delete the elements when deconstructing*/
        explicit Page(PointRef pos, const std::vector<ElementType *> elements, bool independent = false)
                : WidgetParent{pos},
                  independent(independent) {
            for (auto &ele: elements) {
                this->elements.insert({ele, ele->get_type()});
            }
        }

        ~Page() override {
            if (independent)
                for (auto &ele: elements)
                    delete ele.first;
        }

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_page);
            auto ele_rel = this->pos + rel;
            for (auto &ele: elements) {
                auto widget_result = new WidgetResult();
                ele.first->process(ele_rel, mgr, *widget_result);
                result.result.page->insert({ele.first, widget_result});
            }
        }

        WIDGET_PRESENT override {
            auto ele_rel = this->pos + rel;
            for (auto &ele: elements) {
                ele.first->present(renderer, ele_rel);
            }
        }

        WIDGET_TYPE(WidgetResult::t_page);
    };

    using MousePage = Page<MouseMgr>;
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETPAGE_HPP
