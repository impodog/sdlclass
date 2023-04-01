//
// Created by Dogs-Cute on 3/25/2023.
//

#ifndef SDLCLASS_EXTCOLOR_HPP
#define SDLCLASS_EXTCOLOR_HPP

#include "ExtBase.h"

#define COLOR_OP(op, x, val, max_min) {              \
    auto ok = op(x,val);                             \
    if (ok.ok)                                       \
        x = ok.value;                                \
    else                                             \
        x = std::numeric_limits<typeof x>::max_min();\
}
#define COLOR_OPS(op, val, max_min) COLOR_OP(op, r, val, max_min) COLOR_OP(op, g, val, max_min) COLOR_OP(op, b, val, max_min)

#define COLOR_MERGE_OP(op, x, val, max_min) {        \
    auto ok = op(x/2, val/2);                        \
    if (ok.ok)                                       \
        x = ok.value;                                \
    else                                             \
        x = std::numeric_limits<typeof x>::max_min();\
}

#define COLOR_MERGE_OPS(op, color, max_min) COLOR_MERGE_OP(op, r, color.r, max_min) COLOR_MERGE_OP(op, g, color.g, max_min) COLOR_MERGE_OP(op, b, color.b, max_min)

#define rgb(r, g, b) Color{r,g,b}

NS_BEGIN
    /*Extension subclass of SDL_Color*/
    class Color : public SDL_Color {
    protected:
    public:
        Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0) : SDL_Color{r, g, b, a} {}

        Color &operator+(Uint8 dim_val) {
            COLOR_OPS(add_ok, dim_val, max)
            return *this;
        }

        Color &operator+(const Color &color) {
            COLOR_MERGE_OPS(add_ok, color, max)
            return *this;
        }

        Color &operator-(Uint8 dim_val) {
            COLOR_OPS(sub_ok, dim_val, min)
            return *this;
        }

        Color &operator-(const Color &color) {
            COLOR_MERGE_OPS(sub_ok, color, max)
            return *this;
        }

        Color &operator-() {
            r = max_of(r) - r;
            g = max_of(g) - g;
            b = max_of(b) - b;
            return *this;
        }

        Color &operator=(const Color &) = default;

        Color &operator=(Color &&) = default;
    };

    using ConstColorRef = const Color &;
NS_END

#undef COLOR_OP
#undef COLOR_OPS
#undef COLOR_MERGE_OP
#undef COLOR_MERGE_OPS

#endif //SDLCLASS_EXTCOLOR_HPP
