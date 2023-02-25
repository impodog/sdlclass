//
// Created by Dogs-Cute on 2/25/2023.
//

#ifndef SDLCLASS_H
#define SDLCLASS_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

using Window = SDL_Window *;
using Renderer = SDL_Renderer *;
using FontPtr = TTF_Font *;
using Color = SDL_Color;
using TexturePtr = SDL_Texture *;
using SurfacePtr = SDL_Surface *;

Renderer renderer;
Window window;

template<class XYType>
class XYPair {
public:
    using PairType = XYPair<XYType>;
    using PairRef = const PairType &;
    XYType x, y;

    XYPair(XYType x, XYType y) noexcept {
        this->x = x;
        this->y = y;
    }

    PairType operator+(PairRef pair) const {
        return {x + pair.x, y + pair.y};
    }

    PairType operator-(PairRef pair) const {
        return {x - pair.x, y - pair.y};
    }

    PairType operator*(XYType m) const {
        return {x * m, y * m};
    }

    PairType operator/(XYType m) const {
        return {x / m, y / m};
    }

    PairType &operator+=(PairRef pair) {
        x += pair.x;
        y += pair.y;
        return *this;
    }

    PairType &operator-=(PairRef pair) {
        x -= pair.x;
        y -= pair.y;
        return *this;
    }

    PairType &operator*=(XYType m) {
        x *= m;
        y *= m;
        return *this;
    }

    PairType &operator/=(XYType m) {
        x *= m;
        y *= m;
        return *this;
    }

    template<class XYCastType>
    operator XYPair<XYCastType>() { // NOLINT(google-explicit-constructor)
        return {static_cast<XYCastType>(x), static_cast<XYCastType>(y)};
    }
};

using IntPair = XYPair<int>;
using DoublePair = XYPair<long double>;

class Rect : public SDL_Rect {
protected:
    using PairRef = IntPair::PairRef;
public:
    bool null;

    Rect() noexcept: SDL_Rect{0, 0, 0, 0} { null = true; }

    Rect(int x, int y, int w, int h) noexcept: SDL_Rect{x, y, w, h} { null = false; }

    explicit Rect(PairRef size) noexcept: SDL_Rect{0, 0, size.x, size.y} { null = false; }

    explicit Rect(SurfacePtr surface) noexcept: SDL_Rect{0, 0, surface->w, surface->h} { null = false; }

    Rect(SurfacePtr surface, PairRef shift) noexcept: SDL_Rect{shift.x, shift.y, surface->w,
                                                               surface->h} { null = false; }

    ~Rect() = default;

    Rect &operator=(const Rect &rect) {
        x = rect.x;
        y = rect.y;
        w = rect.w;
        h = rect.h;
        null = rect.null;
        return *this;
    }

    [[nodiscard]] IntPair center() const {
        return {x + w / 2, y + h / 2};
    }

    [[nodiscard]] Rect expand_copy(long double m, PairRef pos) const {
        return {static_cast<int>((x - pos.x) * m + pos.x), static_cast<int>((y - pos.y) * m + pos.y),
                static_cast<int>(w * m), static_cast<int>(h * m)};
    }

    Rect &expand(long double m, PairRef pos) {
        x = static_cast<int>((x - pos.x) * m + pos.x);
        y = static_cast<int>((y - pos.y) * m + pos.y);
        w = static_cast<int>(w * m);
        h = static_cast<int>(h * m);
        return *this;
    }

    Rect operator+(PairRef pair) const {
        return {x + pair.x, y + pair.y, w, h};
    }

    Rect operator-(PairRef pair) const {
        return {x - pair.x, y - pair.y, w, h};
    }

    Rect operator*(long double m) const {
        return expand_copy(m, center());
    }

    Rect operator/(long double m) const {
        return expand_copy(1 / m, center());
    }

    Rect &operator+=(PairRef pair) {
        x += pair.x;
        y += pair.y;
        return *this;
    }

    Rect &operator-=(PairRef pair) {
        x -= pair.x;
        y -= pair.y;
        return *this;
    }

    Rect &operator*=(long double m) {
        return expand(m, center());
    }

    Rect &operator/=(long double m) {
        return expand(1 / m, center());
    }

    constexpr operator const SDL_Rect *() const { // NOLINT(google-explicit-constructor)
        return null ? nullptr : this;
    }
};

class Texture {
protected:
    TexturePtr texture;
    bool independent;
public:
    Rect srcrect;

    Texture() noexcept {
        texture = nullptr;
        independent = false;
    }

    Texture(const Texture &a_texture) noexcept {
        texture = a_texture.texture;
        srcrect = a_texture.srcrect;
        independent = false;
    }

    Texture(TexturePtr a_texture, bool a_independent = true) noexcept { // NOLINT(google-explicit-constructor)
        texture = a_texture;
        independent = a_independent;
    }

    Texture(SurfacePtr a_surface) noexcept { // NOLINT(google-explicit-constructor)
        texture = SDL_CreateTextureFromSurface(renderer, a_surface);
        srcrect = Rect(a_surface);
        independent = true;
    }

    Texture &operator=(const Texture &) = delete;

    Texture &operator=(Texture &&a_surface) noexcept {
        texture = a_surface.texture;
        independent = a_surface.independent;
        return *this;
    }

    ~Texture() {
        if (independent) SDL_DestroyTexture(texture);
    }

    void copy(IntPair::PairRef dst) {
        SDL_RenderCopy(renderer, texture, srcrect, srcrect + dst);
    }

    void copy(Rect a_dstrect) {
        SDL_RenderCopy(renderer, texture, srcrect, a_dstrect);
    }

    operator TexturePtr() { // NOLINT(google-explicit-constructor)
        return texture;
    }
};

class Surface {
protected:
    SurfacePtr surface;
    bool independent;
public:
    Surface() noexcept {
        surface = nullptr;
        independent = false;
    }

    Surface(const Surface &a_surface) noexcept {
        surface = a_surface.surface;
        independent = false;
    }

    Surface(SurfacePtr a_surface, bool a_independent = true) noexcept { // NOLINT(google-explicit-constructor)
        surface = a_surface;
        independent = a_independent;
    }

    explicit Surface(const std::string &file) noexcept {
        surface = IMG_Load(file.c_str());
        independent = true;
    }

    Surface(int width, int height, int depth = 32, Uint32 Rmask = 0, Uint32 Gmask = 0, Uint32 Bmask = 0,
            Uint32 Amask = 0) noexcept {
        surface = SDL_CreateRGBSurface(0, width, height, depth, Rmask, Gmask, Bmask, Amask);
        independent = true;
    }

    ~Surface() {
        if (independent) SDL_FreeSurface(surface);
    }

    Surface &operator=(const Surface &) = delete;

    Surface &operator=(Surface &&a_surface) noexcept {
        surface = a_surface.surface;
        independent = a_surface.independent;
        return *this;
    }

    void copy(IntPair::PairRef dst) {
        Texture texture = surface;
        texture.copy(dst);
    }

    void copy(Rect a_dstrect) {
        Texture texture = surface;
        texture.copy(a_dstrect);
    }

    void blit(Surface &a_surface, const Rect &srcrect = {}, Rect dstrect = {}) {
        SDL_BlitSurface(a_surface, srcrect, surface, &dstrect);
    }

    void set_color_key(const Color &color, int flag = SDL_TRUE) {
        SDL_SetColorKey(surface, flag, get_color(color));
    }

    void set_alpha(Uint8 alpha) {
        SDL_SetSurfaceAlphaMod(surface, alpha);
    }

    void set_blend(SDL_BlendMode blendMode) {
        SDL_SetSurfaceBlendMode(surface, blendMode);
    }

    Uint32 get_color(const Color &color) {
        return SDL_MapRGB(surface->format, color.r, color.g, color.b);
    }

    void fill_rect(const Color &color, const Rect &rect = {}) {
        SDL_FillRect(surface, rect, get_color(color));
    }

    operator SurfacePtr() { // NOLINT(google-explicit-constructor)
        return surface;
    }
};

class Font {
protected:
    FontPtr font;
public:
    Font(const std::string &file, int size) {
        font = TTF_OpenFont(file.c_str(), size);
    }

    ~Font() {
        TTF_CloseFont(font);
    }

    Font(const Font &) = delete;

    Font &operator=(const Font &) = delete;

    Surface render(const std::string &text, const Color &color) {
        return TTF_RenderUTF8_Blended(font, text.c_str(), color);
    }

    Surface render_wrapped(const std::string &text, const Color &color, Uint32 wrapLength = 0) {
        return TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrapLength);
    }

    Surface render_solid(const std::string &text, const Color &color) {
        return TTF_RenderUTF8_Solid(font, text.c_str(), color);
    }

    Surface render_solid_wrapped(const std::string &text, const Color &color, Uint32 wrapLength = 0) {
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

    Font &operator[](int size) {
        try {
            return get(size);
        } catch (const std::out_of_range &) {
            return create(size);
        }
    }

    Font &get(int size) {
        return *fonts.at(size);
    }

    Font &create(int size) {
        auto new_font = new Font{file, size};
        fonts.insert({size, new_font});
        return *new_font;
    }
};

#endif //SDLCLASS_H
