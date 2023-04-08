//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETINPUTBOX_HPP
#define SDLCLASS_TEST_EXTWIDGETINPUTBOX_HPP

#include <utility>

#include "ExtBase.h"
#include "ExtColor.hpp"
#include "ExtWidget.hpp"

NS_BEGIN
    WIDGET_TEMPLATE(=MouseAndKeyClickMgr<>)

    class InputBox final : public WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        using OnClickPred = bool (*)(const MgrType &, const Rect &, bool);
        using CharInput = bool (*)(const MgrType &, std::string &);

        std::string input;
        Point size, background_rel, text_rel;
        Font &font;
        const Color &font_color;
        NumType outline_size;
        SDLSurfacePtr background;
        ConstSchemeRef scheme;

        Surface *back = nullptr, *text = nullptr;
        bool is_front = false;

        OnClickPred on_click_pred;
        CharInput char_input;

        void draw_front() {
            delete back;
            back = new Surface(size);
            back->fill_rect(scheme.front_outline);
            back->fill_rect(scheme.front_body,
                            Rect{outline_size, outline_size,
                                 size.x - outline_size * 2, size.y - outline_size * 2});
        }

        void draw_back() {
            delete back;
            back = new Surface(size);
            back->fill_rect(scheme.back_outline);
            back->fill_rect(scheme.back_body,
                            Rect{outline_size, outline_size,
                                 size.x - outline_size * 2, size.y - outline_size * 2});
        }

        void draw_background() {
            if (background != nullptr)
                back->blit(background, background_rel);
        }

        void rerender_text() {
            delete text;
            text = new Surface(font.render_wrapped(input, font_color, size.x - outline_size * 2));
        }

        static typename std::enable_if<std::is_same<MgrType, MouseAndKeyClickMgr<>>::value, bool>::type
        mouse_on_click(const MouseAndKeyClickMgr<> &mgr, const Rect &box_rect, bool is_front) {
            if (is_front) {
                return mgr.mouse.is_click(MouseMgr::left);
            } else {
                return mgr.mouse.is_click(MouseMgr::left) &&
                       box_rect.contains(mgr.mouse.at(MouseMgr::pos)) &&
                       box_rect.contains(mgr.mouse.at(MouseMgr::left));
            }
        }

        static typename std::enable_if<std::is_same<MgrType, MouseAndKeyClickMgr<>>::value, bool>::type
        keyboard_char_input(const MouseAndKeyClickMgr<> &mgr, std::string &input) {
            switch (mgr.cur_key) {
                case 0:
                    break;
                case '\b':
                    if (!input.empty())
                        input.pop_back();
                    break;
                case '\n':
                    return true;
                default:
                    if (SDL_isprint(mgr.cur_key))
                        input.push_back((char) mgr.cur_key);
                    break;
            }
            return false;
        }

    public:
/**
 * \param pos the position kept by \c InputBox
 * \param size the size of the \c InputBox
 * \param font the font of the input
 * \param font_color the color of the input
 * \param outline_size the outline size of the \c InputBox. Defaults to 1
 * \param default_input the input text that's already in the box when initializing. Defaults to empty string
 * \param input_pos the pointer of position of the input text. If NULL, then the input will be shown at the left. Defaults to \c nullptr
 * \param background the background shown when no input is present in the InputBox. Defaults to \c nullptr
 * \param background_pos the pointer of position of the background(if usable). If NULL, then the background will be shown at the center. Defaults to \c nullptr
 * \param scheme the scheme that the InputBox uses. Defaults to scheme_bright
 * \param on_click_pred this is a function that returns true when a press change is detected. Defaults to \c mouse_on_click
 * \param char_input this is a function that pushes or pops the input char according to key activities. Defaults to \c keyboard_char_input*/
        InputBox(PointRef pos, PointRef size, Font &font, const Color &font_color, NumType outline_size = 1,
                 std::string default_input = {}, const Point *input_pos = nullptr,
                 SDLSurfacePtr background = nullptr, const Point *background_pos = nullptr,
                 ConstSchemeRef scheme = scheme_bright,
                 OnClickPred on_click_pred = mouse_on_click, CharInput char_input = keyboard_char_input) :
                WidgetParent{pos}, size(size), font(font), font_color(font_color),
                outline_size(outline_size), input(std::move(default_input)),
                background(background), scheme(scheme),
                on_click_pred(on_click_pred), char_input(char_input) {
            if (background != nullptr) {
                if (background_pos == nullptr)
                    background_rel = {(size.x - background->w) / 2, (size.y - background->h) / 2};
                else this->background_rel = *background_pos;
            }
            if (input_pos == nullptr)
                text_rel = {outline_size * 2, (size.y - font.height()) / 2};
            else text_rel = *input_pos;
            draw_back();
        }

        ~InputBox() override {
            delete back;
            delete text;
        }

        bool to_front() {
            draw_front();
            is_front = true;
            return true;
        }

        bool to_back() {
            draw_back();
            is_front = false;
            return true;
        }

        WIDGET_DELETES(InputBox)

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_input_box);
            Point real = this->pos + rel;
            if (on_click_pred(mgr, {real.x, real.y, size.x, size.y}, is_front)) {
                if (is_front) to_back();
                else to_front();
            }
            if (is_front) {
                result.result.input_box.end = char_input(mgr, input);
                if (mgr.cur_key != 0)
                    rerender_text();
            }
            if (input.empty())
                draw_background();
            result.result.input_box.str = &input;
        }

        WIDGET_PRESENT override {
            Point real = this->pos + rel;
            back->copy_to(renderer, this->pos + rel);
            if (!(text == nullptr || text->is_null()))
                text->copy_to(renderer, this->pos + text_rel + rel);
        }

        WIDGET_TYPE(WidgetResult::t_input_box)
    };

    using DefaultInputBox = InputBox<>;
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETINPUTBOX_HPP
