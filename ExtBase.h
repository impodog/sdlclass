//
// Created by Dogs-Cute on 3/25/2023.
//

#ifndef SDLCLASS_EXTMACROS_H
#define SDLCLASS_EXTMACROS_H

#include "SDLClass.h"
#include <limits>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <ranges>
#include "SDL2_rotozoom.h"

#define NS_BEGIN namespace SDLExt {
#define NS_END }

#define DEFAULT_POINT {0, 0}
#define VOID_POINT {-1, -1}

#define max_of(x) std::numeric_limits<x>::max()
#define min_of(x) std::numeric_limits<x>::min()
#define epsilon_of(x) std::numeric_limits<x>::epsilon()
#define lim_of(x, lim) std::numeric_limits<x>::lim()

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
#define WIDGET_PRESENT void present(Renderer & renderer, const Point &rel)
#define WIDGET_DELETES(type) type(const type &) = delete;\
type &operator=(type &&) = delete;                       \
type &operator=(const type &) = delete;

#ifndef UNDEF_MACROS
#define UNDEF_MACROS false
#endif // UNDEF_MACROS

NS_BEGIN
    using namespace SDLClass;
NS_END

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