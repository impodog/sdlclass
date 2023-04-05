//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETGENERATE_HPP
#define SDLCLASS_TEST_EXTWIDGETGENERATE_HPP

#include "ExtBase.h"
#include "ExtWidgetButton.hpp"
#include "ExtWidgetPage.hpp"

NS_BEGIN
    namespace Generate {
/**
 * Creates a line of buttons, vertical or horizontal
 * \param style the button style(See \c Button())
 * \param begin_pos the beginning position of all those passed as parameters to the buttons
 * \param images the images passed to the button. This defines how many buttons are created
 * \param sep the separation between buttons*/

        WIDGET_TEMPLATE(=MouseMgr, bool vertical = true)
        typename std::enable_if<vertical, std::vector<Button<MgrType> *>>::type
        generate_buttons(
                const Point &begin_pos, const typename Button<MgrType>::ButtonStyle &style,
                const std::vector<SDLSurfacePtr> &images, int sep = 0) {
            std::vector<Button<MgrType> *> result;
            Point pos = begin_pos;
            for (auto img: images) {
                result.push_back(new Button<MgrType>(pos, style, img));
                pos.y += style.size.y + sep;
            }
            return result;
        }
/**
 * Creates a line of buttons, vertical or horizontal
 * \param style the button style(See \c Button())
 * \param begin_pos the beginning position of all those passed as parameters to the buttons
 * \param images the images passed to the button. This defines how many buttons are created
 * \param sep the separation between buttons*/
        WIDGET_TEMPLATE(=MouseMgr, bool vertical = true)
        typename std::enable_if<!vertical, std::vector<Button<MgrType> *>>::type
        generate_buttons(
                const Point &begin_pos, const typename Button<MgrType>::ButtonStyle &style,
                const std::vector<SDLSurfacePtr> &images, int sep = 0) {
            std::vector<Button<MgrType> *> result;
            Point pos = begin_pos;
            for (auto img: images) {
                result.push_back(new Button<MgrType>(pos, style, img));
                pos.x += style.size.x + sep;
            }
            return result;
        }

        WIDGET_TEMPLATE()
        Page<MgrType> wrap_with_page(WidgetBase<MgrType> *widget, bool independent = false) {
            return {widget->view_pos(), {widget}, independent};
        }
    }
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETGENERATE_HPP
