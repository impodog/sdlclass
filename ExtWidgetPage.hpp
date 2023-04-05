//
// Created by Dogs-Cute on 4/4/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETPAGE_HPP
#define SDLCLASS_TEST_EXTWIDGETPAGE_HPP

#include "ExtBase.h"
#include "ExtWidget.hpp"

NS_BEGIN
/*A Page of contents that can manage every component repeatedly*/
    WIDGET_TEMPLATE()
    class Page : WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        using ElementType = WidgetBase<MgrType>;
        std::unordered_map<ElementType *, WidgetResult::WidgetType> elements;
        bool independent;
    public:
        /**
         * \param elements a vector of Widgets with the same manager to be put on this page
         * \param independent whether the page will delete the elements when deconstructing*/
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

        WIDGET_DELETES(Page)

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
    template<typename KeyType = SDL_Keycode>
    using MouseAndKeyPage = Page<MouseAndKeyClickMgr<KeyType>>;

    WIDGET_TEMPLATE()
    class BranchPage : WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        using BranchMap = std::unordered_map<Button<MgrType> *, Page<MgrType> *>;
        BranchMap branches;
        Button<MgrType> *cur_branch;
        bool ind_button = false, ind_page = false;
    public:
        BranchPage(PointRef pos, BranchMap branch_map,
                   Button<MgrType> *init_branch = nullptr, bool ind_button = false, bool ind_page = false
        ) :
                WidgetParent{pos},
                branches(std::move(branch_map)),
                ind_button(ind_button), ind_page(ind_page) {
            if (branches.empty())
                throw std::out_of_range("BranchPage constructor received no available branches");
            if (init_branch == nullptr)
                cur_branch = branches.begin()->first;
            else {
                if (branches.find(init_branch) == branches.end())
                    throw std::out_of_range(
                            "BranchPage constructor parameter init_branch is not one of the keys of branches");
                cur_branch = init_branch;
            }
        }

        BranchPage(PointRef pos, const std::vector<Button<MgrType> *> &keys,
                   const std::vector<Page<MgrType> *> &values, Button<MgrType> *init_branch = nullptr) :
                WidgetParent{pos} {
            size_t size = keys.size();
            if (keys.empty())
                throw std::out_of_range("BranchPage constructor received no available branches");
            else if (size != values.size())
                throw std::out_of_range("BranchPage constructor did not receive the same size of keys and values");
            if (init_branch == nullptr)
                cur_branch = *keys.begin();
            else {
                if (std::find(keys.begin(), keys.end(), cur_branch) == keys.end())
                    throw std::out_of_range(
                            "BranchPage constructor parameter init_branch is not one of the keys of branches");
                cur_branch = init_branch;
            }
            auto key_iter = keys.begin();
            auto value_iter = values.begin();
            for (size_t i = 0; i < size; i++)
                branches.insert({*key_iter++, *value_iter++});
        }

        ~BranchPage() override {
            for (auto &pair: branches) {
                if (ind_button)
                    delete pair.first;
                if (ind_page)
                    delete pair.second;
            }
        }

        void add_branch(const std::pair<Button<MgrType> *, Page<MgrType> *> &pair) {
            branches.insert(pair);
        }

        void erase_branch(Button<MgrType> *key) {
            branches.erase(key);
            if (branches.empty())
                throw std::out_of_range("All available branches are deleted in BranchPage");
            if (cur_branch == key)
                cur_branch = branches.begin()->first;
        }

        WIDGET_DELETES(BranchPage)

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_branch_page);
            WidgetResult sub_result;
            for (auto &pair: branches) {
                pair.first->process(rel, mgr, sub_result);
                if (sub_result.result.button.released)
                    cur_branch = pair.first;
            }
            result.result.branch_page->key = cur_branch;
            branches.at(cur_branch)->process(rel, mgr, sub_result);
            result.result.branch_page->page_res = *sub_result.result.page;
            sub_result.result.page->clear();
        }

        WIDGET_PRESENT override {
            for (auto &pair: branches)
                pair.first->present(renderer, rel);
            branches.at(cur_branch)->present(renderer, rel);
        }

        WIDGET_TYPE(WidgetResult::t_branch_page)
    };
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETPAGE_HPP
