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

namespace SDLClass {

    using WindowPtr = SDL_Window *;
    using RendererPtr = SDL_Renderer *;
    using FontPtr = TTF_Font *;
    using SDLTexturePtr = SDL_Texture *;
    using SDLSurfacePtr = SDL_Surface *;
    using ConstSDLTexturePtr = const SDL_Texture *;
    using ConstSDLSurfacePtr = const SDL_Surface *;
    using Event = SDL_Event;

    template<typename SDLPointType>
    class BasicPoint : public SDLPointType {
    public:
        using NumType = typeof
        SDLPointType::x;
        using PointType = BasicPoint<SDLPointType>;
        using PointRef = const PointType &;

        BasicPoint() : SDLPointType{0, 0} {};

        BasicPoint(NumType x, NumType y) noexcept {
            this->x = x;
            this->y = y;
        }

        BasicPoint(const SDLPointType &point) noexcept { // NOLINT(google-explicit-constructor)
            this->x = point.x;
            this->y = point.y;
        }

        PointType operator+(PointRef point) const {
            return {this->x + point.x, this->y + point.y};
        }

        PointType operator-(PointRef point) const {
            return {this->x - point.x, this->y - point.y};
        }

        PointType operator*(NumType m) const {
            return {this->x * m, this->y * m};
        }

        PointType operator/(NumType m) const {
            return {this->x / m, this->y / m};
        }

        PointType &operator+=(PointRef point) {
            this->x += point.x;
            this->y += point.y;
            return *this;
        }

        PointType &operator-=(PointRef point) {
            this->x -= point.x;
            this->y -= point.y;
            return *this;
        }

        PointType &operator*=(NumType m) {
            this->x *= m;
            this->y *= m;
            return *this;
        }

        PointType &operator/=(NumType m) {
            this->x *= m;
            this->y *= m;
            return *this;
        }

        [[nodiscard]] constexpr NumType size() const {
            return this->x * this->y;
        }

        PointType operator-() const {
            return {-this->x, -this->y};
        }

        [[nodiscard]] NumType distance(PointRef point) const {
            return sqrt(pow(point.x - this->x, 2) + pow(point.y - this->y, 2));
        }

        [[nodiscard]] constexpr bool positive() const {
            return this->x > 0 && this->y > 0;
        }

        [[nodiscard]] constexpr bool negative() const {
            return this->x < 0 && this->y < 0;
        }

        [[nodiscard]] constexpr bool is0() const {
            return this->x == 0 && this->y == 0;
        }

        [[nodiscard]] constexpr bool isn0() const {
            return this->x != 0 || this->y != 0;
        }

        void to_positive() {
            this->x = abs(this->x);
            this->y = abs(this->y);
        }

        void to_negative() {
            this->x = -abs(this->x);
            this->y = -abs(this->y);
        }

        void to0() {
            this->x = this->y = 0;
        }

        void invert() {
            this->x = -this->x;
            this->y = -this->y;
        }

        template<class NumCastType>
        operator BasicPoint<NumCastType>() { // NOLINT(google-explicit-constructor)
            return {static_cast<NumCastType>(this->x), static_cast<NumCastType>(this->y)};
        }

        operator SDL_Point() { // NOLINT(google-explicit-constructor)
            return {static_cast<int>(this->x), static_cast<int>(this->y)};
        }

        operator SDL_FPoint() { // NOLINT(google-explicit-constructor)
            return {static_cast<float>(this->x), static_cast<float>(this->y)};
        }
    };

    using Point = BasicPoint<SDL_Point>;
    using FPoint = BasicPoint<SDL_FPoint>;

    template<class SDLPointType>
    std::ostream &operator<<(std::ostream &ostream, const BasicPoint<SDLPointType> &point) {
        ostream << "(" << point.x << "," << point.y << ")";
        return ostream;
    }

    template<typename SDLRectType, typename SDLPointType>
    class BasicRect : public SDLRectType {
    protected:
        using NumType = typename std::enable_if<std::is_same< typeof
        SDLRectType::x,
        typeof SDLPointType::x
        >::value,
        typeof SDLRectType::x
        >::type;
        using PointType = BasicPoint<SDLPointType>;
        using PointRef = typename PointType::PointRef;
        using RectType = BasicRect<SDLRectType, SDLPointType>;
        using RectRef = const RectType &;
    public:
        bool null;

        BasicRect() noexcept: SDLRectType{0, 0, 0, 0} { null = true; }

        BasicRect(int x, int y, int w, int h) noexcept: SDLRectType{x, y, w, h} { null = false; }

        explicit BasicRect(PointRef size) noexcept: SDLRectType{0, 0, size.x, size.y} { null = false; }

        BasicRect(PointRef begin, PointRef end) noexcept: SDLRectType{begin.x, begin.y,
                                                                      end.x - begin.x,
                                                                      end.y - begin.y} { null = false; }

        explicit BasicRect(SDLSurfacePtr surface) noexcept: SDLRectType{0, 0, surface->w, surface->h} { null = false; }

        BasicRect(SDLSurfacePtr surface, PointRef shift) noexcept: SDLRectType{shift.x, shift.y, surface->w,
                                                                               surface->h} { null = false; }

        ~BasicRect() = default;

        RectType &operator=(RectRef rect) {
            this->x = rect.x;
            this->y = rect.y;
            this->w = rect.w;
            this->h = rect.h;
            null = rect.null;
            return *this;
        }

        [[nodiscard]] PointType center() const {
            return {this->x + this->w / 2, this->y + this->h / 2};
        }

        [[nodiscard]] constexpr PointType lu() const {
            return {this->x, this->y};
        }

        [[nodiscard]] constexpr PointType ld() const {
            return {this->x, this->y + this->h};
        }

        [[nodiscard]] constexpr PointType ru() const {
            return {this->x + this->w, this->y};
        }

        [[nodiscard]] constexpr PointType rd() const {
            return {this->x + this->w, this->y + this->h};
        }

        [[nodiscard]] RectType expand_copy(long double m, PointRef pos) const {
            return {static_cast<int>((this->x - pos.x) * m + pos.x), static_cast<int>((this->y - pos.y) * m + pos.y),
                    static_cast<int>(this->w * m), static_cast<int>(this->h * m)};
        }

        RectType &expand(long double m, PointRef pos) {
            this->x = static_cast<int>((this->x - pos.x) * m + pos.x);
            this->y = static_cast<int>((this->y - pos.y) * m + pos.y);
            this->w = static_cast<int>(this->w * m);
            this->h = static_cast<int>(this->h * m);
            return *this;
        }

        [[nodiscard]] constexpr PointType inters_size(RectRef rect) const {
            return {this->w + rect.w - abs(this->x - rect.x), this->h + rect.h - abs(this->y - rect.y)};
        }

        [[nodiscard]] RectType inters_rect(RectRef rect) const {
            PointType this_rd = rd(), rect_rd = rect.rd();
            return {{std::max(this->x, rect.x),      std::max(this->y, rect.y)},
                    {std::min(this_rd.x, rect_rd.x), std::min(this_rd.y, rect_rd.y)}};
        }

        IF_RECT_TYPE(SDL_Rect, bool)
        empty() const {
            return SDL_RectEmpty(this);
        }

        IF_RECT_TYPE(SDL_FRect, bool)
        empty() const {
            return SDL_FRectEmpty(this);
        }

        IF_RECT_TYPE(SDL_Rect, bool)
        contains(const Point &point) const {
            return SDL_PointInRect(&point, this);
        }

        IF_RECT_TYPE(SDL_FRect, bool)
        contains(const FPoint &point) const {
            return SDL_PointInFRect(&point, this);
        }

        [[nodiscard]] constexpr bool positive() const {
            return this->w > 0 && this->h > 0;
        }

        [[nodiscard]] constexpr bool negative() const {
            return this->w < 0 && this->h < 0;
        }

        void to_empty() {
            this->w = this->h = 0;
        }

        [[nodiscard]] constexpr bool eq_positive() const {
            return this->w >= 0 && this->h >= 0;
        }

        [[nodiscard]] constexpr bool eq_negative() const {
            return this->w <= 0 && this->h <= 0;
        }

        void to_positive() {
            if (this->w < 0) {
                this->x += this->w;
                this->w = -this->w;
            }
            if (this->h < 0) {
                this->y += this->h;
                this->h = -this->h;
            }
        }

        void invert() {
            this->x += this->w;
            this->w = -this->w;
            this->y += this->h;
            this->h = -this->h;
        }

        RectType operator+(PointRef point) const {
            return {this->x + point.x, this->y + point.y, this->w, this->h};
        }

        RectType operator-(PointRef point) const {
            return {this->x - point.x, this->y - point.y, this->w, this->h};
        }

        RectType operator*(long double m) const {
            return expand_copy(m, center());
        }

        RectType operator/(long double m) const {
            return expand_copy(1 / m, center());
        }

        RectType &operator+=(PointRef point) {
            this->x += point.x;
            this->y += point.y;
            return *this;
        }

        RectType &operator-=(PointRef point) {
            this->x -= point.x;
            this->y -= point.y;
            return *this;
        }

        RectType &operator*=(long double m) {
            return expand(m, center());
        }

        RectType &operator/=(long double m) {
            return expand(1 / m, center());
        }

        constexpr bool operator==(RectRef rect) const {
            return SDL_RectEquals(this, rect);
        }

        constexpr operator const SDLRectType *() const { // NOLINT(google-explicit-constructor)
            return null ? nullptr : this;
        }

        operator SDLRectType *() { // NOLINT(google-explicit-constructor)
            return null ? nullptr : this;
        }

        operator SDL_Rect() { // NOLINT(google-explicit-constructor)
            return {static_cast<int>(this->x), static_cast<int>(this->y),
                    static_cast<int>(this->w), static_cast<int>(this->h)};
        }

        operator SDL_FRect() { // NOLINT(google-explicit-constructor)
            return {static_cast<float>(this->x), static_cast<float>(this->y),
                    static_cast<float>(this->w), static_cast<float>(this->h)};
        }
    };

    template<class SDLRectType, class SDLPointType>
    std::ostream &operator<<(std::ostream &ostream, const BasicRect<SDLRectType, SDLPointType> &rect) {
        ostream << "(" << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << ")";
        return ostream;
    }

    using Rect = BasicRect<SDL_Rect, SDL_Point>;
    using FRect = BasicRect<SDL_FRect, SDL_FPoint>;

    class Texture {
    protected:
        SDLTexturePtr texture;
        bool independent;
    public:
        Rect srcrect;

        Texture() noexcept: texture(nullptr), independent(false) {}

        Texture(const Texture &texture) noexcept: texture(texture.texture), srcrect(texture.srcrect),
                                                  independent(false) {}

        Texture(SDLTexturePtr texture, bool independent = true) noexcept:// NOLINT(google-explicit-constructor)
                texture(texture), independent(independent) {}

        Texture(RendererPtr renderer, SDLSurfacePtr surface) noexcept: srcrect(surface), independent(
                true) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
        } // NOLINT(google-explicit-constructor

        Texture &operator=(const Texture &a_texture) noexcept {
            texture = a_texture.texture;
            independent = false;
            return *this;
        }

        Texture &operator=(Texture &&a_texture) noexcept {
            texture = a_texture.texture;
            independent = a_texture.independent;
            return *this;
        }

        ~Texture() {
            if (independent) SDL_DestroyTexture(texture);
        }

        void copy_to(RendererPtr renderer, Point::PointRef dst) {
            SDL_RenderCopy(renderer, texture, srcrect, srcrect + dst);
        }

        void copy_to(RendererPtr renderer, Rect a_dstrect) {
            SDL_RenderCopy(renderer, texture, srcrect, a_dstrect);
        }

        void set_blend(SDL_BlendMode blendMode) {
            SDL_SetTextureBlendMode(texture, blendMode);
        }

        void set_alpha(Uint8 alpha) {
            SDL_SetTextureAlphaMod(texture, alpha);
        }

        operator SDLTexturePtr() { // NOLINT(google-explicit-constructor)
            return texture;
        }
    };

    class Surface {
    public:
        SDLSurfacePtr surface;
        bool independent;

        Surface() noexcept: surface(nullptr), independent(false) {}

        Surface(Surface &surface) {
            this->surface = surface.surface;
            independent = false;
        }

        Surface(Surface &surface, const SDL_PixelFormat *fmt) noexcept: surface(
                SDL_ConvertSurface(surface, fmt, 0)), independent(true) {}

        Surface(SDLSurfacePtr surface, bool independent = true) noexcept:// NOLINT(google-explicit-constructor)
                surface(surface), independent(independent) {}

        explicit Surface(const std::string &file) noexcept: surface(IMG_Load(file.c_str())), independent(true) {}

        Surface(int width, int height, int depth = 32, Uint32 Rmask = 0, Uint32 Gmask = 0, Uint32 Bmask = 0,
                Uint32 Amask = 0) noexcept: surface(
                SDL_CreateRGBSurface(0, width, height, depth, Rmask, Gmask, Bmask, Amask)), independent(true) {}

        explicit Surface(const Point &size, int depth = 32, Uint32 Rmask = 0, Uint32 Gmask = 0, Uint32 Bmask = 0,
                         Uint32 Amask = 0) noexcept: surface(
                SDL_CreateRGBSurface(0, size.x, size.y, depth, Rmask, Gmask, Bmask, Amask)), independent(true) {}


        ~Surface() {
            free();
        }

        Surface &operator=(const Surface &a_surface) noexcept {
            free();
            surface = a_surface.surface;
            independent = false;
            return *this;
        }

        Surface &operator=(Surface &&a_surface) noexcept {
            free();
            surface = a_surface.surface;
            independent = a_surface.independent;
            a_surface.independent = false;
            return *this;
        }

        [[nodiscard]] constexpr bool is_null() const noexcept {
            return surface == nullptr;
        }

        [[nodiscard]] Point size() const noexcept {
            return Point{surface->w, surface->h};
        }

        // This function is safe to call with nullptr and will eventually set surface to nullptr
        void free() noexcept {
            if (independent) {
                SDL_FreeSurface(surface);
                surface = nullptr;
            }
        }

        void copy_to(RendererPtr renderer, Point::PointRef dst) {
            Texture texture{renderer, surface};
            texture.copy_to(renderer, dst);
        }

        void copy_to(RendererPtr renderer, Rect a_dstrect) {
            Texture texture{renderer, surface};
            texture.copy_to(renderer, a_dstrect);
        }

        void blit(SDLSurfacePtr a_surface, const Point &dst) {
            SDL_BlitSurface(a_surface, nullptr, surface, Rect(a_surface) + dst);
        }

        void blit(SDLSurfacePtr a_surface, SDL_Rect *dstrect = nullptr) {
            SDL_BlitSurface(a_surface, nullptr, surface, dstrect);
        }

        void blit(SDLSurfacePtr a_surface, const SDL_Rect *srcrect, SDL_Rect *dstrect) {
            SDL_BlitSurface(a_surface, srcrect, surface, dstrect);
        }

        void lower_blit(SDLSurfacePtr a_surface, SDL_Rect *srcrect, SDL_Rect *dstrect) {
            SDL_LowerBlit(a_surface, srcrect, surface, dstrect);
        }

        void set_color_key(const SDL_Color &color, int flag = SDL_TRUE) {
            SDL_SetColorKey(surface, flag, get_color(color));
        }

        void set_alpha(Uint8 alpha) {
            SDL_SetSurfaceAlphaMod(surface, alpha);
        }

        void set_blend(SDL_BlendMode blendMode) {
            SDL_SetSurfaceBlendMode(surface, blendMode);
        }

        void lock() {
            SDL_LockSurface(surface);
        }

        void unlock() {
            SDL_UnlockSurface(surface);
        }

        Uint32 get_color(const SDL_Color &color) {
            return SDL_MapRGB(surface->format, color.r, color.g, color.b);
        }

        void fill_rect(const SDL_Color &color, const SDL_Rect *rect = nullptr) {
            SDL_FillRect(surface, rect, get_color(color));
        }

        operator SDLSurfacePtr() { // NOLINT(google-explicit-constructor)
            return surface;
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
}

#undef IF_RECT_TYPE

#endif //SDLCLASS_H
