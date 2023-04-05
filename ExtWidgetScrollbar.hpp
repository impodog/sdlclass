//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETSCROLLBAR_HPP
#define SDLCLASS_TEST_EXTWIDGETSCROLLBAR_HPP

#include "ExtBase.h"
#include "ExtWidget.hpp"
#include "ExtWidgetButton.hpp"

#define IF_VERT(spec, typen) template<bool vertical_=vertical> \
spec typename std::enable_if<vertical_, typen>::type
#define IF_HORIZ(spec, typen) template<bool vertical_=vertical> \
spec typename std::enable_if<!vertical_, typen>::type

#define IF_VERT_AND(spec, typen, cond...) template<bool vertical_=vertical> \
spec typename std::enable_if<vertical_ && cond, typen>::type
#define IF_HORIZ_AND(spec, typen, cond...) template<bool vertical_=vertical> \
spec typename std::enable_if<!vertical_ && cond, typen>::type

#define IF_VERT_AND_TMPL(spec, typen, tmpl, cond...) template<bool vertical_=vertical,tmpl> \
spec typename std::enable_if<vertical_ && cond, typen>::type
#define IF_HORIZ_AND_TMPL(spec, typen, tmpl, cond...) template<bool vertical_=vertical,tmpl> \
spec typename std::enable_if<!vertical_ && cond, typen>::type

NS_BEGIN
/*A bar, horizontal or vertical, with a slide button on it.*/
    WIDGET_TEMPLATE(=MouseMgr, bool vertical = true)
    class Scrollbar : public WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        using DragInput = long double (*)(const MgrType &, const Rect &, PointRef, bool);
        Point real_size, scroll_size, button_rel;
        Button<MgrType> *button;
        long double percentage;

        ConstSchemeRef scheme;

        DragInput drag_input;
        Surface *back = nullptr;

        IF_VERT(, void)
        set_button_pos() {
            button_rel = {0, static_cast<NumType>(scroll_size.y * percentage)};
        }

        IF_HORIZ(, void)
        set_button_pos() {
            button_rel = {static_cast<NumType>(scroll_size.x * percentage), 0};
        }

        IF_VERT(, Point)
        get_button_size(NumType but_len) const {
            return {real_size.x, but_len};
        }

        IF_HORIZ(, Point)
        get_button_size(NumType but_len) const {
            return {but_len, real_size.y};
        }

        IF_VERT(, void)
        set_scroll_size(NumType but_len) {
            scroll_size = {real_size.x, real_size.y - but_len};
        }

        IF_HORIZ(, void)
        set_scroll_size(NumType but_len) {
            scroll_size = {real_size.x - but_len, real_size.y};
        }

        IF_VERT_AND(static, long double, std::is_same<MgrType, MouseMgr>::value)
        mouse_drag_input(const MouseMgr &mgr, const Rect &but_rect, PointRef size, bool but_is_front) {
            if (mgr.moved() && mgr.is_down(MouseMgr::left) &&
                (but_rect.contains(mgr.at(MouseMgr::left)) || but_is_front)) {
                return static_cast<long double>(mgr.at(MouseMgr::move).y) / size.y;
            }
            return 0;
        }

        IF_HORIZ_AND(static, long double, std::is_same<MgrType, MouseMgr>::value)
        mouse_drag_input(const MouseMgr &mgr, const Rect &but_rect, PointRef size, bool but_is_front) {
            if (mgr.moved() && mgr.is_down(MouseMgr::left) &&
                (but_rect.contains(mgr.at(MouseMgr::left)) || but_is_front)) {
                return static_cast<long double>(mgr.at(MouseMgr::move).x) / size.x;
            }
            return 0;
        }

        IF_VERT_AND_TMPL(static, long double, typename KeyType,
                         std::is_same<MgrType, MouseAndKeyClickMgr<KeyType>>::value)
        mouse_drag_input(const MouseAndKeyClickMgr<KeyType> &mgr, const Rect &but_rect, PointRef size,
                         bool but_is_front) {
            if (mgr.mouse.moved() && mgr.mouse.is_down(MouseMgr::left) &&
                (but_rect.contains(mgr.mouse.at(MouseMgr::left)) || but_is_front)) {
                return static_cast<long double>(mgr.mouse.at(MouseMgr::move).y) / size.y;
            }
            return 0;
        }

        IF_HORIZ_AND_TMPL(static, long double, typename KeyType,
                          std::is_same<MgrType, MouseAndKeyClickMgr<KeyType>>::value)
        mouse_drag_input(const MouseAndKeyClickMgr<KeyType> &mgr, const Rect &but_rect, PointRef size,
                         bool but_is_front) {
            if (mgr.mouse.moved() && mgr.mouse.is_down(MouseMgr::left) &&
                (but_rect.contains(mgr.mouse.at(MouseMgr::left)) || but_is_front)) {
                return static_cast<long double>(mgr.mouse.at(MouseMgr::move).x) / size.x;
            }
            return 0;
        }

    public:
/**
 * \attention Call \c this->InitButton() after initializing function
 * \param pos the position kept by \c Scrollbar
 * \param size the size of the \c Scrollbar
 * \param init_percentage the default percentage on initializing. Defaults to 0
 * \param outline_size the size in pixels of outline. Defaults to 1
 * \param background the background of the bar itself. Defaults to \c nullptr
 * \param background_pos the const pointer of a point of where the background should be shown relatively. If \c NULL, then in the center. Defaults to \c nullptr
 * \param scheme the color scheme that the \c Scrollbar uses. Defaults to \c scheme_bright
 * \param drag_input a function processing how the \c Scrollbar should respond to dragging and return the percentage changes. Defaults to mouse_drag_input(if usable)
 * */
        Scrollbar(PointRef pos, PointRef size, long double init_percentage = 0,
                  NumType outline_size = 1, SDLSurfacePtr background = nullptr,
                  const Point *background_pos = nullptr, ConstSchemeRef scheme = scheme_bright,
                  DragInput drag_input = mouse_drag_input) :
                WidgetParent{pos}, real_size(size), percentage(init_percentage),
                scheme(scheme), drag_input(drag_input) {
            set_button_pos();
            back = new Surface(size);
            back->fill_rect(scheme.front_outline);
            back->fill_rect(scheme.front_body,
                            Rect{outline_size, outline_size,
                                 size.x - outline_size * 2, size.y - outline_size * 2});
            if (background != nullptr) {
                Point background_rel;
                if (background_pos == nullptr)
                    background_rel = {(size.x - background->w) / 2, (size.y - background->h) / 2};
                else background_rel = *background_pos;
                back->blit(background, background_rel);
            }
        }

        // See Button constructor for more
        auto InitButton(NumType len, NumType outline_size = 1, SDLSurfacePtr img = nullptr,
                        const Point *img_pos = nullptr, ConstSchemeRef but_scheme = scheme_dark,
                        Button<MgrType>::PressedPred pred = Button<MgrType>::mouse_pressed) {
            button = new Button<MgrType>(this->pos, get_button_size(len), outline_size, img, img_pos, but_scheme, pred);
            set_scroll_size(len);
            return this;
        }


        ~Scrollbar() {
            delete back;
            delete button;
        }

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_scrollbar);
            auto drag = drag_input(mgr, button->get_rect(button_rel + rel), scroll_size, button->is_front);
            if (drag != 0) {
                percentage += drag;
                if (percentage < 0) percentage = 0;
                else if (percentage > 1) percentage = 1;
            }
            set_button_pos();
            {
                WidgetResult button_result;
                button->process(button_rel + rel, mgr, button_result);
            }
            result.result.scrollbar.percentage = percentage;
        }

        WIDGET_PRESENT override {
            back->copy_to(renderer, this->pos + rel);
            button->present(renderer, button_rel + rel);
        }

        WIDGET_TYPE(WidgetResult::t_scrollbar)
    };

    using MouseVertScrollbar = Scrollbar<MouseMgr, true>;
    using MouseHorizScrollbar = Scrollbar<MouseMgr, false>;
    template<typename KeyType=SDL_Keycode>
    using MouseAndKeyVertScrollbar = Scrollbar<MouseAndKeyClickMgr<KeyType>, true>;
    template<typename KeyType=SDL_Keycode>
    using MouseAndKeyHorizScrollbar = Scrollbar<MouseAndKeyClickMgr<KeyType>, false>;
NS_END

#undef IF_VERT
#undef IF_HORIZ
#undef IF_VERT_AND
#undef IF_HORIZ_AND
#undef IF_VERT_AND_TMPL
#undef IF_HORIZ_AND_TMPL

#endif //SDLCLASS_TEST_EXTWIDGETSCROLLBAR_HPP
