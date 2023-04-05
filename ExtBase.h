//
// Created by Dogs-Cute on 3/25/2023.
//

#ifndef SDLCLASS_EXTMACROS_H
#define SDLCLASS_EXTMACROS_H

#include "SDLClass.h"
#include <limits>
#include <iostream>
#include <vector>

#define NS_BEGIN namespace SDLExt {
#define NS_END }

#define DEFAULT_POINT {0, 0}
#define VOID_POINT {-1, -1}

#define sign(x) (x>0?1:x<0?-1:0)
#define max_of(x) std::numeric_limits<typeof x>::max()
#define min_of(x) std::numeric_limits<typeof x>::min()
#define epsilon_of(x) std::numeric_limits<typeof x>::epsilon()
#define lim_of(x, lim) std::numeric_limits<typeof x>::lim()

#define WIDGET_TEMPLATE(additional...) template<typename MgrType additional>
#define WIDGET_PARENT WidgetBase<MgrType>
#define WIDGET_TYPEDEFS using WidgetParent = WIDGET_PARENT;\
using NumType = WidgetParent::NumType;\
using PointRef = WidgetParent::PointRef;                   \
using ConstSchemeRef = WidgetParent::ConstSchemeRef;
#define WIDGET_TYPEDEFS_WITHOUT_PARENT using NumType = WidgetParent::NumType;\
using PointRef = WidgetParent::PointRef;                   \
using ConstSchemeRef = WidgetParent::ConstSchemeRef;
#define WIDGET_TYPE(type)[[nodiscard]] constexpr WidgetResult::WidgetType get_type() const noexcept override { return type; }
#define WIDGET_PROCESS void process(const Point &rel, const MgrType &mgr, WidgetResult &result)
#define WIDGET_PRESENT void present(RendererPtr renderer, const Point &rel)

NS_BEGIN
    using namespace SDLClass;

    template<class NumType>
    struct Ok {
        NumType value;
        bool ok;
    };

    template<class NumType>
    typename std::enable_if<std::is_unsigned<NumType>::value, Ok<NumType>>::type
    add_ok(NumType a, NumType b) noexcept {
        NumType sum = a + b;
        return {sum, (sum > a) && (sum > b)};
    }

    template<class NumType>
    typename std::enable_if<std::is_signed<NumType>::value, Ok<NumType>>::type
    add_ok(NumType a, NumType b) noexcept {
        int sign_a = sign(a), sign_b = sign(b);
        NumType sum = a + b;
        return {sum, (sign_a != sign_b) || (sign_a == sum)};
    }

    template<class NumType>
    typename std::enable_if<std::is_unsigned<NumType>::value, Ok<NumType>>::type
    sub_ok(NumType a, NumType b) noexcept {
        return {NumType(a - b), a > b};
    }

    template<class NumType>
    typename std::enable_if<std::is_signed<NumType>::value, Ok<NumType>>::type
    sub_ok(NumType a, NumType b) noexcept {
        return add_ok(a, -b);
    }

NS_END

#ifndef UNDEF_MACROS
#define UNDEF_MACROS false
#endif // UNDEF_MACROS

#endif //SDLCLASS_EXTMACROS_H


#if UNDEF_MACROS

#undef NS_BEGIN
#undef NS_END

#undef WIDGET_TEMPLATE
#undef WIDGET_PARENT
#undef WIDGET_TYPEDEFS
#undef WIDGET_TYPE
#undef WIDGET_PROCESS
#undef WIDGET_PRESENT

#undef UNDEF_MACROS

#endif // UNDEF_MACROS