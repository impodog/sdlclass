//
// Created by Dogs-Cute on 4/1/2023.
//

#ifndef SDLCLASS_EXTKEYMGR_HPP
#define SDLCLASS_EXTKEYMGR_HPP

#define MOUSE_BUTTON_TYPE decltype(SDL_Event().button.button)
#define MOUSE_CLICK_MGR_PARENT KeyClickMgr<MOUSE_BUTTON_TYPE>
#define MOUSE_KEYS std::initializer_list<MOUSE_BUTTON_TYPE>{0,1,2,3,4,5}

#include "ExtBase.h"

NS_BEGIN
/*A simple manager for keys down/up from all ranges*/
    template<typename KeyType>
    class KeyMgr {
    protected:
        using KeyMap = std::unordered_map<KeyType, bool>;
        KeyMap key_down;
    public:
        KeyMgr(std::initializer_list<KeyType> init) {
            for (const auto &key: init)
                key_down.insert({key, false});
        }

        void down_unchecked(const KeyType &key) noexcept {
            key_down[key] = true;
        }

        void down(const KeyType &key) {
            key_down.at(key) = true;
        }

        void up_unchecked(const KeyType &key) noexcept {
            key_down[key] = false;
        }

        void up(const KeyType &key) {
            key_down.at(key) = false;
        }

        constexpr bool is_down(const KeyType &key) const {
            return key_down.at(key);
        }

        constexpr bool is_down_unchecked(const KeyType &key) const noexcept {
            return key_down[key];
        }

        constexpr bool is_up(const KeyType &key) const {
            return key_down[key];
        }

        constexpr bool is_up_unchecked(const KeyType &key) const noexcept {
            return !key_down[key];
        }
    };

/*Subclass of KeyMgr, but added records for key clicked
 * NOTICE : record() MUST be called before down() and up(); refresh() must be called after the key management*/
    template<typename KeyType>
    class KeyClickMgr : public KeyMgr<KeyType> {
    protected:
        using KeyMap = typename KeyMgr<KeyType>::KeyMap;
        KeyMap key_prev, key_click, key_release;
    public:
        KeyClickMgr(std::initializer_list<KeyType> init) : KeyMgr<KeyType>(init) {
            for (const auto &key: init) {
                key_prev.insert({key, false});
                key_click.insert({key, false});
                key_release.insert({key, false});
            }
        }

        void record() {
            key_prev = this->key_down;
        }

        void refresh() {
            for (const auto &key: key_prev) {
                auto &k_d = this->key_down.at(key.first);
                if (k_d ^ key.second) {
                    key_click.at(key.first) = k_d;
                    key_release.at(key.first) = !k_d;
                }
            }
        }

        void refresh_unchecked() {
            for (const auto &key: key_prev) {
                auto &k_d = this->key_down[key.first];
                if (k_d ^ key.second) {
                    key_click[key.first] = k_d;
                    key_release[key.first] = !k_d;
                }
            }
        }
    };

    class MouseMgr : public MOUSE_CLICK_MGR_PARENT {
    protected:
    public:
        using MgrParent = MOUSE_CLICK_MGR_PARENT;
        using ButtonType = MOUSE_BUTTON_TYPE;
        std::unordered_map<ButtonType, Point> position;

        static const int pos = 0, left = 1, middle = 2, right = 3, scroll_up = 4, scroll_down = 5;

        MouseMgr() : MgrParent(MOUSE_KEYS) {
            for (auto key: MOUSE_KEYS)
                position.insert({key, VOID_POINT});
        }

        void motion(const SDL_MouseButtonEvent &button) {
            position.at(pos) = {button.x, button.y};
        }

        void down(const SDL_MouseButtonEvent &button) {
            MgrParent::down(button.button);
            position.at(button.button) = {button.x, button.y};
        }

        void down_unchecked(const SDL_MouseButtonEvent &button) noexcept {
            MgrParent::down_unchecked(button.button);
            position[button.button] = {button.x, button.y};
        }

        void up(const SDL_MouseButtonEvent &button) {
            MgrParent::up(button.button);
            position.at(button.button) = VOID_POINT;
        }

        void up_unchecked(const SDL_MouseButtonEvent &button) noexcept {
            MgrParent::up_unchecked(button.button);
            position[button.button] = VOID_POINT;
        }

        const Point &at(ButtonType button) const {
            return position.at(button);
        }

        Point &at_unchecked(ButtonType button) {
            return position[button];
        }

        Point &operator[](ButtonType button) {
            return position[button];
        }
    };

NS_END

#undef MOUSE_BUTTON_TYPE
#undef MOUSE_CLICK_MGR_PARENT
#undef MOUSE_KEYS

#endif //SDLCLASS_EXTKEYMGR_HPP
