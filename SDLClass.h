//
// Created by Dogs-Cute on 2/25/2023.
//

#ifndef SDLCLASS_H
#define SDLCLASS_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#define IF_RECT_TYPE(rect_type, return_type) template<class SDLRectPointType = SDLRectType>\
    [[nodiscard]] constexpr typename std::enable_if<std::is_same<SDLRectPointType, rect_type>::value, return_type>::type
#define NOT_ANY_RECT_TYPE(return_type) template<class SDLRectPointType = SDLRectType> \
    [[nodiscard]] constexpr typename std::enable_if<!std::is_same<SDLRectPointType, SDL_Rect &&>::value && !std::is_same<SDLRectPointType, SDL_Rect>::value, return_type>::type

namespace SDLClass {

    using WindowPtr = SDL_Window *;
    using SDLRendererPtr = SDL_Renderer *;
    using FontPtr = TTF_Font *;
    using SDLTexturePtr = SDL_Texture *;
    using SDLSurfacePtr = SDL_Surface *;
    using ConstSDLTexturePtr = const SDL_Texture *;
    using ConstSDLSurfacePtr = const SDL_Surface *;
    using Event = SDL_Event;

    template<typename NumType>
    struct FakePointType {
        static_assert(std::is_arithmetic<NumType>::value, "FakePointType::NumType must be an arithmetic type");
        NumType x, y;
    };
    template<typename NumType>
    struct FakeRectType {
        static_assert(std::is_arithmetic<NumType>::value, "FakeRectType::NumType must be an arithmetic type");
        NumType x, y;
        NumType w, h;
    };

#if __cplusplus >= 202002L
    template<typename SDLPointType>
    concept HasXY_ = std::is_arithmetic<
    typeof SDLPointType::x
    >
    ::value &&
            std::is_same<
    typeof SDLPointType::x,
    typeof SDLPointType::y
    >::value;
#define HasXY HasXY_
#define TypeHasXY(T) HasXY<T>
#else
#define HasXY typename
#define TypeHasXY(T) true
#endif

    template<HasXY SDLPointType>
    class BasicPoint : public SDLPointType {
    public:
        using NumType = typeof
        SDLPointType::x;
        static_assert(std::is_arithmetic<NumType>::value, "BasicPoint number type must be arithmetic");
        using PointType = BasicPoint<SDLPointType>;
        using PointRef = const PointType &;

        BasicPoint() noexcept: SDLPointType{0, 0} {};

        BasicPoint(NumType x, NumType y) noexcept {
            this->x = x;
            this->y = y;
        }

        BasicPoint(const SDLPointType &point) noexcept { // NOLINT(google-explicit-constructor)
            this->x = point.x;
            this->y = point.y;
        }

        PointType operator+(PointRef point) const noexcept {
            return {this->x + point.x, this->y + point.y};
        }

        PointType operator-(PointRef point) const noexcept {
            return {this->x - point.x, this->y - point.y};
        }

        PointType operator*(NumType m) const noexcept {
            return {this->x * m, this->y * m};
        }

        PointType operator/(NumType m) const noexcept {
            return {this->x / m, this->y / m};
        }

        PointType &operator+=(PointRef point) noexcept {
            this->x += point.x;
            this->y += point.y;
            return *this;
        }

        PointType &operator-=(PointRef point) noexcept {
            this->x -= point.x;
            this->y -= point.y;
            return *this;
        }

        PointType &operator*=(NumType m) noexcept {
            this->x *= m;
            this->y *= m;
            return *this;
        }

        PointType &operator/=(NumType m) noexcept {
            this->x /= m;
            this->y /= m;
            return *this;
        }

        [[nodiscard]] constexpr NumType size() const noexcept {
            return this->x * this->y;
        }

        PointType operator-() const noexcept {
            return {-this->x, -this->y};
        }

        [[nodiscard]] constexpr bool operator==(PointRef point) const noexcept = default;

        [[nodiscard]] constexpr NumType scalar_product(PointRef point) const noexcept {
            return this->x * point.x + this->y * point.y;
        }

        [[nodiscard]] constexpr NumType vector_product(PointRef point) const noexcept {
            return this->x * point.y - this->y * point.x;
        }

        [[nodiscard]] NumType distance(PointRef point) const noexcept {
            return sqrt(pow(point.x - this->x, 2) + pow(point.y - this->y, 2));
        }

        [[nodiscard]] BasicPoint<SDL_Point> to_IPoint() const noexcept {
            return {static_cast<int>(this->x), static_cast<int>(this->y)};
        }

        [[nodiscard]] constexpr bool positive() const noexcept {
            return this->x > 0 && this->y > 0;
        }

        [[nodiscard]] constexpr bool negative() const noexcept {
            return this->x < 0 && this->y < 0;
        }

        [[nodiscard]] constexpr bool is0() const noexcept {
            return this->x == 0 && this->y == 0;
        }

        [[nodiscard]] constexpr bool isn0() const noexcept {
            return this->x != 0 || this->y != 0;
        }

        void to_positive() noexcept {
            this->x = abs(this->x);
            this->y = abs(this->y);
        }

        void to_negative() noexcept {
            this->x = -abs(this->x);
            this->y = -abs(this->y);
        }

        void to0() noexcept {
            this->x = this->y = 0;
        }

        void invert() noexcept {
            this->x = -this->x;
            this->y = -this->y;
        }

        constexpr operator SDL_Point() const noexcept { // NOLINT(google-explicit-constructor)
            return {static_cast<int>(this->x), static_cast<int>(this->y)};
        }

        constexpr operator SDL_FPoint() const noexcept { // NOLINT(google-explicit-constructor)
            return {static_cast<float>(this->x), static_cast<float>(this->y)};
        }

        template<HasXY NumCastType>
        constexpr operator BasicPoint<NumCastType>() const noexcept { // NOLINT(google-explicit-constructor)
            return {static_cast<typeof NumCastType::x > (this->x), static_cast<typeof NumCastType::x > (this->y)};
        }
    };

    using Point = BasicPoint<SDL_Point>;
    using FPoint = BasicPoint<SDL_FPoint>;
    template<typename NumType>
        // A template Point that has the same functionality as any other Point
    using ArithPoint = BasicPoint<FakePointType<NumType>>;

    template<HasXY SDLPointType>
    std::ostream &operator<<(std::ostream &ostream, const BasicPoint<SDLPointType> &point) {
        ostream << "(" << point.x << "," << point.y << ")";
        return ostream;
    }

#if __cplusplus >= 202002L
    template<typename SDLRectType>
    concept HasXYWH_ = TypeHasXY(SDLRectType) && std::is_same<
    typeof SDLRectType::x,
    typeof SDLRectType::w
    >
    ::value &&
            std::is_same<
    typeof SDLRectType::w,
    typeof SDLRectType::h
    >::value;

#define HasXYWH HasXYWH_
#define TypeHasXYWH(T) HasXYWH<T>

#else
#define HasXYWH typename
#define typeHasXYWH(T) true
#endif

    template<HasXYWH SDLRectType, HasXY SDLPointType>
    class BasicRect : public SDLRectType {
    protected:
        using NumType = typename std::enable_if<std::is_same< typeof
        SDLRectType::x,
        typeof SDLPointType::x
        >::value,
        typeof SDLRectType::x
        >::type;
        static_assert(std::is_arithmetic<NumType>::value, "BasicRect number type must be arithmetic");
        using PointType = BasicPoint<SDLPointType>;
        using PointRef = typename PointType::PointRef;
        using RectType = BasicRect<SDLRectType, SDLPointType>;
        using RectRef = const RectType &;
    public:

        BasicRect() noexcept: SDLRectType{0, 0, 0, 0} {}

        BasicRect(NumType x, NumType y, NumType w, NumType h) noexcept: SDLRectType{x, y, w, h} {}

        explicit BasicRect(PointRef size) noexcept: SDLRectType{0, 0, size.x, size.y} {}

        explicit BasicRect(SDLSurfacePtr surface) noexcept: SDLRectType{0, 0, surface->w, surface->h} {}

        BasicRect(SDLSurfacePtr surface, PointRef shift) noexcept: SDLRectType{shift.x, shift.y, surface->w,
                                                                               surface->h} {}

        ~BasicRect() noexcept = default;

        static RectType from_to(PointRef from, PointRef to) noexcept {
            return {from.x, from.y, to.x - from.x, to.y - from.y};
        }

        static RectType pos_size(PointRef pos, PointRef size) noexcept {
            return {pos.x, pos.y, size.x, size.y};
        }

        RectType &operator=(RectRef rect) noexcept {
            this->x = rect.x;
            this->y = rect.y;
            this->w = rect.w;
            this->h = rect.h;
            return *this;
        }

        [[nodiscard]] PointType center() const noexcept {
            return {this->x + this->w / 2, this->y + this->h / 2};
        }

        [[nodiscard]] constexpr PointType lu() const noexcept {
            return {this->x, this->y};
        }

        [[nodiscard]] constexpr PointType ld() const noexcept {
            return {this->x, this->y + this->h};
        }

        [[nodiscard]] constexpr PointType ru() const noexcept {
            return {this->x + this->w, this->y};
        }

        [[nodiscard]] constexpr PointType rd() const noexcept {
            return {this->x + this->w, this->y + this->h};
        }

        [[nodiscard]] RectType expand_copy(double m, PointRef pos) const noexcept {
            return {static_cast<NumType>((this->x - pos.x) * m + pos.x),
                    static_cast<NumType>((this->y - pos.y) * m + pos.y),
                    static_cast<NumType>(this->w * m), static_cast<NumType>(this->h * m)};
        }

        RectType &expand(double m, PointRef pos) noexcept {
            this->x = static_cast<NumType>((this->x - pos.x) * m + pos.x);
            this->y = static_cast<NumType>((this->y - pos.y) * m + pos.y);
            this->w = static_cast<NumType>(this->w * m);
            this->h = static_cast<NumType>(this->h * m);
            return *this;
        }

        // If this is positive, then there is intersection.
        [[nodiscard]] constexpr PointType inters_size(RectRef rect) const noexcept {
            return {this->w + rect.w - abs(this->x - rect.x), this->h + rect.h - abs(this->y - rect.y)};
        }

        // If this is positive, then there is intersection.
        [[nodiscard]] RectType inters_rect(RectRef rect) const noexcept {
            PointType this_rd = rd(), rect_rd = rect.rd();
            return RectType::from_to({std::max(this->x, rect.x), std::max(this->y, rect.y)},
                                     {std::min(this_rd.x, rect_rd.x), std::min(this_rd.y, rect_rd.y)});
        }

        [[nodiscard]] BasicRect<SDL_Rect, SDL_Point> to_IRect() const noexcept {
            return {static_cast<int>(this->x), static_cast<int>(this->y),
                    static_cast<int>(this->w), static_cast<int>(this->h)};
        }

        IF_RECT_TYPE(SDL_Rect, bool)
        empty() const noexcept {
            return SDL_RectEmpty(this);
        }

        IF_RECT_TYPE(SDL_FRect, bool)
        empty() const noexcept {
            return SDL_FRectEmpty(this);
        }

        NOT_ANY_RECT_TYPE(bool)
        empty() const noexcept {
            return this->w == 0 || this->h == 0;
        }

        IF_RECT_TYPE(SDL_Rect, bool)
        contains(const Point &point) const noexcept {
            return SDL_PointInRect(&point, this);
        }

        IF_RECT_TYPE(SDL_FRect, bool)
        contains(const FPoint &point) const noexcept {
            return SDL_PointInFRect(&point, this);
        }

        NOT_ANY_RECT_TYPE(bool)
        contains(const SDLPointType &point) const noexcept {
            return this->x <= point.x && point.x <= this->x + this->w &&
                   this->y <= point.y && point.y <= this->y + this->h;
        }


        [[nodiscard]] constexpr bool positive() const noexcept {
            return this->w > 0 && this->h > 0;
        }

        [[nodiscard]] constexpr bool negative() const noexcept {
            return this->w < 0 && this->h < 0;
        }

        void to_empty() noexcept {
            this->w = this->h = 0;
        }

        [[nodiscard]] constexpr bool eq_positive() const noexcept {
            return this->w >= 0 && this->h >= 0;
        }

        [[nodiscard]] constexpr bool eq_negative() const noexcept {
            return this->w <= 0 && this->h <= 0;
        }

        void to_positive() noexcept {
            if (this->w < 0) {
                this->x += this->w;
                this->w = -this->w;
            }
            if (this->h < 0) {
                this->y += this->h;
                this->h = -this->h;
            }
        }

        void invert() noexcept {
            this->x += this->w;
            this->w = -this->w;
            this->y += this->h;
            this->h = -this->h;
        }

        RectType operator+(PointRef point) const noexcept {
            return {this->x + point.x, this->y + point.y, this->w, this->h};
        }

        RectType operator-(PointRef point) const noexcept {
            return {this->x - point.x, this->y - point.y, this->w, this->h};
        }

        RectType operator*(long double m) const noexcept {
            return expand_copy(m, center());
        }

        RectType operator/(long double m) const noexcept {
            return expand_copy(1 / m, center());
        }

        RectType &operator+=(PointRef point) noexcept {
            this->x += point.x;
            this->y += point.y;
            return *this;
        }

        RectType &operator-=(PointRef point) noexcept {
            this->x -= point.x;
            this->y -= point.y;
            return *this;
        }

        RectType &operator*=(long double m) noexcept {
            return expand(m, center());
        }

        RectType &operator/=(long double m) noexcept {
            return expand(1 / m, center());
        }

        constexpr bool operator==(RectRef rect) const noexcept {
            return SDL_RectEquals(this, rect);
        }

        constexpr bool operator!=(RectRef rect) const noexcept {
            return !SDL_RectEquals(this, rect);
        }

        constexpr operator SDL_Rect() const noexcept { // NOLINT(google-explicit-constructor)
            return {static_cast<int>(this->x), static_cast<int>(this->y),
                    static_cast<int>(this->w), static_cast<int>(this->h)};
        }

        constexpr operator SDL_FRect() const noexcept { // NOLINT(google-explicit-constructor)
            return {static_cast<float>(this->x), static_cast<float>(this->y),
                    static_cast<float>(this->w), static_cast<float>(this->h)};
        }

        template<HasXYWH SDLRectType_, HasXY SDLPointType_>
        constexpr operator BasicRect<SDLRectType_, SDLPointType_>() // NOLINT(google-explicit-constructor)
        const noexcept {
            using NumType_ = typeof
            BasicRect<SDLRectType_, SDLPointType_>::x;
            return {static_cast<NumType_>(this->x), static_cast<NumType_>(this->y),
                    static_cast<NumType_>(this->w), static_cast<NumType_>(this->h)};
        }

        constexpr operator const SDLRectType *() const noexcept { // NOLINT(google-explicit-constructor)
            return this;
        }

        constexpr operator SDLRectType *() noexcept { // NOLINT(google-explicit-constructor)
            return this;
        }
    };

    template<HasXYWH SDLRectType, HasXY SDLPointType>
    std::ostream &operator<<(std::ostream &ostream, const BasicRect<SDLRectType, SDLPointType> &rect) {
        ostream << "(" << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << ")";
        return ostream;
    }

    using Rect = BasicRect<SDL_Rect, SDL_Point>;
    using FRect = BasicRect<SDL_FRect, SDL_FPoint>;

    template<typename NumType>
    using ArithRect = BasicRect<FakeRectType<NumType>, FakePointType<NumType>>;

#if __cplusplus >= 202002L
    template<typename T>
    concept CanBePointOrRect_ = std::is_assignable<Point, T>::value || std::is_assignable<Rect, T>::value;
#define CanBePointOrRect CanBePointOrRect_
#define IsPointOrRect(T) CanBePointOrRect_<T>
#else
#define CanBePointOrRect typename
#define IsPointOrRect(T) true
#endif

#define NO_COPY(type) type(const type &) = delete;\
type &operator=(const type &) = delete;           \
type &operator=(type &&) = delete;

    class Renderer {
    protected:
        SDLRendererPtr renderer;
    public:
        Renderer(WindowPtr window, int index, Uint32 flags) : renderer(SDL_CreateRenderer(window, index, flags)) {}

        virtual ~Renderer() {
            destroy();
        }

        NO_COPY(Renderer)

        void destroy() noexcept {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        [[nodiscard]] constexpr SDLRendererPtr ptr() const noexcept {
            return renderer;
        }

        void copy(SDLSurfacePtr surface, const SDL_Rect *srcrect, const SDL_Rect *dstrect) {
            auto texture = SDL_CreateTextureFromSurface(renderer, surface);
            copy(texture, srcrect, dstrect);
        }

        void copy(SDLTexturePtr texture, const SDL_Rect *srcrect = nullptr, const SDL_Rect *dstrect = nullptr) {
            SDL_RenderCopy(renderer, texture, srcrect, dstrect);
        }

        int set_color(const SDL_Color &color) const {
            return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        }

        int clear() const {
            return SDL_RenderClear(renderer);
        }

        int set_logical_size(Point::PointRef size) const {
            return SDL_RenderSetLogicalSize(renderer, size.x, size.y);
        }

        void present() const {
            SDL_RenderPresent(renderer);
        }

        constexpr operator SDLRendererPtr() const noexcept { // NOLINT(google-explicit-constructor)
            return ptr();
        }
    };

    class TextureBase {
    protected:
        SDLTexturePtr texture;
    public:

        TextureBase() noexcept: texture(nullptr) {}

        TextureBase(SDLTexturePtr texture) noexcept:// NOLINT(google-explicit-constructor)
                texture(texture) {}

        virtual ~TextureBase() = default;

        NO_COPY(TextureBase)

        void destroy() noexcept {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        [[nodiscard]] constexpr SDLTexturePtr ptr() const noexcept {
            return texture;
        }

        void copy_to(Renderer &renderer, const SDL_Rect *dstrect = nullptr) {
            renderer.copy(texture, nullptr, dstrect);
        }

        void copy_to(Renderer &renderer, const SDL_Rect *srcrect, const SDL_Rect *dstrect) {
            renderer.copy(texture, srcrect, dstrect);
        }

        void set_alpha(Uint8 alpha) {
            SDL_SetTextureAlphaMod(texture, alpha);
        }

        void set_blend(SDL_BlendMode blendMode) {
            SDL_SetTextureBlendMode(texture, blendMode);
        }

        constexpr operator SDLTexturePtr() const noexcept { // NOLINT(google-explicit-constructor)
            return ptr();
        }
    };

    class Texture : public TextureBase {
    public:
        Texture() : TextureBase() {}

        Texture(SDLRendererPtr renderer, SDLSurfacePtr surface) noexcept {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
        } // NOLINT(google-explicit-constructor

        ~Texture() override {
            destroy();
        }
    };

    class SurfaceBase {
    protected:
        SDLSurfacePtr surface;
    public:

        SurfaceBase() noexcept: surface(nullptr) {}

        SurfaceBase(SDLSurfacePtr surface) noexcept:// NOLINT(google-explicit-constructor)
                surface(surface) {}

        virtual ~SurfaceBase() = default;

        NO_COPY(SurfaceBase)

        void free() noexcept {
            SDL_FreeSurface(surface);
            surface = nullptr;
        }

        [[nodiscard]] constexpr SDLSurfacePtr ptr() const noexcept {
            return surface;
        }

        [[nodiscard]] Point size() const noexcept {
            return Point{surface->w, surface->h};
        }

        [[nodiscard]] int w() const noexcept {
            return surface->w;
        }

        [[nodiscard]] int h() const noexcept {
            return surface->h;
        }

        void copy_to(Renderer &renderer, Point::PointRef dst) const {
            Texture texture{renderer, surface};
            texture.copy_to(renderer, Rect{surface, dst});
        }

        void copy_by_center_to(Renderer &renderer, Point::PointRef center) const {
            copy_to(renderer, {center.x - surface->w / 2, center.y - surface->h / 2});
        }

        void copy_to(Renderer &renderer, const SDL_Rect *dstrect) const {
            Texture texture{renderer, surface};
            texture.copy_to(renderer, dstrect);
        }

        void blit(SDLSurfacePtr a_surface, const Point &dst) const {
            Rect dstrect = Rect(a_surface) + dst;
            SDL_BlitSurface(a_surface, nullptr, surface, &dstrect);
        }

        void blit(SDLSurfacePtr a_surface, SDL_Rect *dstrect = nullptr) const {
            SDL_BlitSurface(a_surface, nullptr, surface, dstrect);
        }

        void blit(SDLSurfacePtr a_surface, const SDL_Rect *srcrect, SDL_Rect *dstrect) const {
            SDL_BlitSurface(a_surface, srcrect, surface, dstrect);
        }

        void lower_blit(SDLSurfacePtr a_surface, SDL_Rect *srcrect, SDL_Rect *dstrect) const {
            SDL_LowerBlit(a_surface, srcrect, surface, dstrect);
        }

        void set_color_key(const SDL_Color &color, int flag = SDL_TRUE) {
            SDL_SetColorKey(surface, flag, get_color(color));
        }

        void set_alpha(Uint8 alpha) const {
            SDL_SetSurfaceAlphaMod(surface, alpha);
        }

        void set_blend(SDL_BlendMode blendMode) const {
            SDL_SetSurfaceBlendMode(surface, blendMode);
        }

        void lock() const {
            SDL_LockSurface(surface);
        }

        void unlock() const {
            SDL_UnlockSurface(surface);
        }

        SDLSurfacePtr merge_x(SurfaceBase &a_surface) const {
            SurfaceBase result = {SDL_CreateRGBSurface(0, surface->w + a_surface.surface->w,
                                                       std::max(surface->h, a_surface.surface->h), 32,
                                                       surface->format->Rmask,
                                                       surface->format->Gmask, surface->format->Bmask,
                                                       surface->format->Amask)};
            result.blit(surface, {0, 0});
            result.blit(a_surface, {surface->w, 0});
            return result.surface;
        }

        SDLSurfacePtr merge_y(SurfaceBase &a_surface) const {
            SurfaceBase result = {SDL_CreateRGBSurface(0, std::max(surface->w, a_surface.surface->w),
                                                       surface->h + a_surface.surface->h, 32,
                                                       surface->format->Rmask,
                                                       surface->format->Gmask, surface->format->Bmask,
                                                       surface->format->Amask)};
            result.blit(surface, {0, 0});
            result.blit(a_surface, {0, surface->h});
            return result.surface;
        }

        [[nodiscard]] Uint32 get_color(const SDL_Color &color) const {
            return SDL_MapRGB(surface->format, color.r, color.g, color.b);
        }

        void fill_rect(const SDL_Color &color, const SDL_Rect *rect = nullptr) const {
            SDL_FillRect(surface, rect, get_color(color));
        }

        operator SDLSurfacePtr() const { // NOLINT(google-explicit-constructor)
            return ptr();
        }
    };

    class Surface : public SurfaceBase {
    public:
        Surface() : SurfaceBase() {}

        explicit Surface(const std::string &file) noexcept: SurfaceBase(IMG_Load(file.c_str())) {}

        // The surface IS managed and will be freed on delete.
        explicit Surface(SDLSurfacePtr surface) noexcept: SurfaceBase(surface) {}

        Surface(int width, int height, int depth = 32, Uint32 Rmask = 0, Uint32 Gmask = 0, Uint32 Bmask = 0,
                Uint32 Amask = 0) noexcept:
                SurfaceBase(SDL_CreateRGBSurface(0, width, height, depth, Rmask, Gmask, Bmask, Amask)) {}

        explicit Surface(const Point &size, int depth = 32, Uint32 Rmask = 0, Uint32 Gmask = 0, Uint32 Bmask = 0,
                         Uint32 Amask = 0) noexcept:
                SurfaceBase(SDL_CreateRGBSurface(0, size.x, size.y, depth, Rmask, Gmask, Bmask, Amask)) {}

        ~Surface() override {
            free();
        }
    };

    class Font {
    protected:
        FontPtr font;
    public:
        Font(const std::string &file, int size) {
            font = TTF_OpenFont(file.c_str(), size);
            if (font == nullptr)
                throw std::runtime_error("Cannot create font of file \"" + file + "\" sized " + std::to_string(size));
        }

        ~Font() {
            TTF_CloseFont(font);
        }

        Font(const Font &) = delete;

        Font &operator=(const Font &) = delete;

        [[nodiscard]] int height() const {
            return TTF_FontHeight(font);
        }

        [[nodiscard]] int is_fixed_width() const {
            return TTF_FontFaceIsFixedWidth(font);
        }

        void set_style(int style) {
            TTF_SetFontStyle(font, style);
        }

        void set_outline(int outline) {
            TTF_SetFontOutline(font, outline);
        }

        const char *family_name() {
            return TTF_FontFaceFamilyName(font);
        }

        const char *style_name() {
            return TTF_FontFaceStyleName(font);
        }


        SDLSurfacePtr render(const std::string &text, const SDL_Color &color) {
            return TTF_RenderUTF8_Blended(font, text.c_str(), color);
        }

        SDLSurfacePtr render_wrapped(const std::string &text, const SDL_Color &color, Uint32 wrapLength = 0) {
            return TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrapLength);
        }

        SDLSurfacePtr render_solid(const std::string &text, const SDL_Color &color) {
            return TTF_RenderUTF8_Solid(font, text.c_str(), color);
        }

        SDLSurfacePtr render_solid_wrapped(const std::string &text, const SDL_Color &color, Uint32 wrapLength = 0) {
            return TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), color, wrapLength);
        }
    };

    class FontFamily {
    protected:
        std::string file;
        std::unordered_map<int, Font *> fonts;
    public:
        explicit FontFamily(const std::string &a_file) {
            file = a_file;
        }

        void close(int size) {
            fonts.erase(size);
        }

        Font &get(int size) noexcept {
            try {
                return at(size);
            } catch (const std::out_of_range &) {
                return create(size);
            }
        }

        Font &operator[](int size) noexcept {
            try {
                return at(size);
            } catch (const std::out_of_range &) {
                return create(size);
            }
        }

        Font &at(int size) {
            return *fonts.at(size);
        }

        Font &create(int size) noexcept {
            auto new_font = new Font{file, size};
            fonts.insert({size, new_font});
            return *new_font;
        }
    };

    inline Rect get_display_bounds(int displayIndex = 0) {
        Rect result;
        SDL_GetDisplayBounds(displayIndex, &result);
        return result;
    }

    inline Rect get_display_usable_bounds(int displayIndex = 0) {
        Rect result;
        SDL_GetDisplayUsableBounds(displayIndex, &result);
        return result;
    }
}

#undef IF_RECT_TYPE

#endif //SDLCLASS_H
