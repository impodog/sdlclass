//
// Created by Dogs-Cute on 3/25/2023.
//

#ifndef SDLCLASS_SDLEXT_H
#define SDLCLASS_SDLEXT_H

#define SDL_EXT_VERSION "v0.3.3 "

#include "ExtMath.hpp"
#include "ExtColor.hpp"
#include "ExtKeyMgr.hpp"
#include "ExtWidget.hpp"
#include "ExtWidgetButton.hpp"
#include "ExtWidgetPage.hpp"
#include "ExtWidgetInputBox.hpp"
#include "ExtWidgetScrollbar.hpp"
#include "ExtWidgetWrapper.hpp"
#include "ExtWidgetGenerate.hpp"
#include "ExtFrameArray.hpp"

#ifdef UNDEF_MACROS
#undef UNDEF_MACROS
#endif // UNDEF_MACROS

#define UNDEF_MACROS true

#include "ExtBase.h"

#endif //SDLCLASS_SDLEXT_H
