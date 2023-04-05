//
// Created by Dogs-Cute on 4/1/2023.
//

#ifndef SDLCLASS_EXTWIDGETBUTTON_HPP
#define SDLCLASS_EXTWIDGETBUTTON_HPP

#include "ExtWidget.hpp"

#define BUTTON_PARENT WidgetBase<MouseMgr>
#define ENABLE_IF_DRAGGABLE(ret_type) template <bool draggable_ = draggable> \
typename std::enable_if<draggable_, ret_type>::type

NS_BEGIN
    WIDGET_TEMPLATE(=MouseMgr)
    class Button : public WIDGET_PARENT {
    protected:
        template<typename MgrType_, bool vertical>
        friend class Scrollbar;

        WIDGET_TYPEDEFS
        using PressedPred = bool (*)(const MgrType &, const Rect &, bool);
        Point size, img_rel;
        NumType outline_size;
        Surface img;
        ConstSchemeRef scheme;

        Surface *surface = nullptr;

        PressedPred pressed_pred;

        template<typename MgrType_ = MgrType>
        static typename std::enable_if<std::is_same<MgrType_, MouseMgr>::value, bool>::type
        mouse_pressed(const MouseMgr &mgr, const Rect &but_rect, bool is_front) {
            return mgr.is_down(MouseMgr::left) && but_rect.contains(mgr.at(MouseMgr::pos)) &&
                   (but_rect.contains(mgr.at(MouseMgr::left)) || is_front);
        }

        template<typename MgrType_ = MgrType, typename KeyType = SDL_Keycode>
        static typename std::enable_if<std::is_same<MgrType_, MouseAndKeyClickMgr<KeyType>>::value, bool>::type
        mouse_pressed(const MgrType &mgr, const Rect &but_rect, bool is_front) {
            return mgr.mouse.is_down(MouseMgr::left) && but_rect.contains(mgr.mouse.at(MouseMgr::pos)) &&
                   (but_rect.contains(mgr.mouse.at(MouseMgr::left)) || is_front);
        }

        void draw_front() {
            delete surface;
            surface = new Surface(size);
            surface->fill_rect(scheme.front_outline);
            surface->fill_rect(scheme.front_body,
                               Rect{outline_size, outline_size,
                                    size.x - outline_size * 2, size.y - outline_size * 2});
            if (!img.is_null()) {
                surface->blit(img, img_rel);
            }
        }

        void draw_back() {
            delete surface;
            surface = new Surface(size);
            surface->fill_rect(scheme.back_outline);
            surface->fill_rect(scheme.back_body,
                               Rect{outline_size, outline_size,
                                    size.x - outline_size * 2, size.y - outline_size * 2});
            if (!img.is_null()) {
                surface->blit(img, img_rel);
            }
        }

    public:
        bool is_front = false;

/**
 * \param pos From base class \c WidgetBase, the position kept by \c Button itself.
 * \param size The size of the button.
 * \param outline_size The length for \c Button outline(edges). Defaults to 1.
 * \param img The img shown along with the button. Defaults to \c nullptr.
 * \param img_pos The relative position to the button showing img. If \c NULL, then show the image in the center. Defaults to \c nullptr.
 * \param scheme The color scheme of button as the name shows. Defaults to \c scheme_bright.
 * \param pressed_pred A function returning bool to define whether the button is pressed. Defaults to \c mouse_pressed.*/
        Button(PointRef pos, PointRef size, NumType outline_size = 1,
               SDLSurfacePtr img = nullptr, const Point *img_pos = nullptr,
               ConstSchemeRef scheme = scheme_bright, PressedPred pressed_pred = mouse_pressed) :
                WidgetParent{pos},
                size(size), outline_size(outline_size),
                img(img, false),
                scheme(scheme),
                pressed_pred(pressed_pred) {
            if (img != nullptr) {
                if (img_pos == nullptr) {
                    img_rel = {(size.x - img->w) / 2, (size.y - img->h) / 2};
                } else this->img_rel = *img_pos;
            }
            draw_back();
        }

        ~Button() override {
            delete surface;
        }

        bool to_front() {
            if (is_front) return false;
            draw_front();
            is_front = true;
            return true;
        }

        bool to_back() {
            if (!is_front) return false;
            draw_back();
            is_front = false;
            return true;
        }

        Rect get_rect(PointRef rel) const {
            Point real = this->pos + rel;
            return {real.x, real.y, size.x, size.y};
        }

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_button);
            if (pressed_pred(mgr, get_rect(rel), is_front)) {
                result.result.button.pressed = to_front();
            } else
                result.result.button.released = to_back();
        }

        WIDGET_PRESENT override {
            surface->copy_to(renderer, this->pos + rel);
        }

        WIDGET_TYPE(WidgetResult::t_button);
    };

    using DefaultButton = Button<>;
    template<typename KeyType = SDL_Keycode>
    using MouseAndKeyButton = Button<MouseAndKeyClickMgr<KeyType>>;
NS_END

#undef BUTTON_PARENT

#endif //SDLCLASS_EXTWIDGETBUTTON_HPP
