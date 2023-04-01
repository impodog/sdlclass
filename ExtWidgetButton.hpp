//
// Created by Dogs-Cute on 4/1/2023.
//

#ifndef SDLCLASS_EXTWIDGETBUTTON_HPP
#define SDLCLASS_EXTWIDGETBUTTON_HPP

#include "ExtWidget.hpp"

#define BUTTON_PARENT WidgetBase<MouseClickMgr>

NS_BEGIN
    static const struct ColorScheme {
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

    class Button : public BUTTON_PARENT {
    protected:
        using WidgetParent = BUTTON_PARENT;
        using NumType = typeof(Point::x);
        using PointRef = typename Point::PointRef;
        using ConstSchemeRef = const ColorScheme &;
        Point size, img_rel;
        NumType outline_size;
        Surface img;
        ConstSchemeRef scheme;

        Surface *surface = nullptr;
        bool is_front = false;

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

        Button(PointRef pos, PointRef size, NumType outline_size = 1,
               SDLSurfacePtr img = nullptr, const Point *img_pos = nullptr,
               ConstSchemeRef scheme = scheme_bright) :
                WidgetParent{pos},
                size(size), outline_size(outline_size),
                img(img, false),
                scheme(scheme) {
            if (img != nullptr) {
                if (img_pos == nullptr) {
                    img_rel = {(size.x - img->w) / 2, (size.y - img->h) / 2};
                } else this->img_rel = *img_pos;
            }
            draw_back();
        }

        ~Button() {
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

        void process(const Point &rel, const MouseClickMgr *mgr, WidgetResult &result) override {
            Point real = this->pos + rel;
            result.set(WidgetResult::button);
            if (Rect{real.x, real.y, size.x, size.y}.contains(mgr->at(MouseClickMgr::left))) {
                result.result.button.up = to_front();
            } else
                result.result.button.down = to_back();
        }

        void present(RendererPtr renderer, const Point &rel) override {
            surface->copy_to(renderer, this->pos + rel);
        }
    };
NS_END

#undef BUTTON_PARENT

#endif //SDLCLASS_EXTWIDGETBUTTON_HPP
