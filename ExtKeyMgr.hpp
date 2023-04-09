//
// Created by Dogs-Cute on 4/1/2023.
//

#ifndef SDLCLASS_EXTKEYMGR_HPP
#define SDLCLASS_EXTKEYMGR_HPP

#define KEY_MGR_TYPEDEFS using KeyType = typename KeyMgr<KeyType_>::KeyType;\
using KeyMap = typename KeyMgr<KeyType>::KeyMap;

#define MOUSE_BUTTON_TYPE decltype(SDL_Event().button.button)
#define MOUSE_CLICK_MGR_PARENT KeyClickMgr<MOUSE_BUTTON_TYPE>
#define MOUSE_KEYS std::initializer_list<MOUSE_BUTTON_TYPE>{0,1,2,3,4}
#define MOUSE_KEYS_EXCEPT_0 std::initializer_list<MOUSE_BUTTON_TYPE>{1,2,3,4}

#include "ExtBase.h"

NS_BEGIN
/*A simple manager for keys down/up from all ranges*/
    template<typename KeyType_=SDL_Keycode>
    class KeyMgr {
    protected:
        using KeyType = KeyType_;
        using KeyMap = std::unordered_map<KeyType, bool>;
        KeyMap key_down;
    public:
        explicit KeyMgr(const std::vector<KeyType> &init) {
            for (const auto &key: init)
                key_down.insert({key, false});
        }

        void down_unchecked(const KeyType &key) noexcept {
            key_down[key] = true;
        }

        void down(const KeyType &key) noexcept {
            try {
                key_down.at(key) = true;
            } catch (const std::out_of_range &) {}
        }

        void up_unchecked(const KeyType &key) noexcept {
            key_down[key] = false;
        }

        void up(const KeyType &key) noexcept {
            try {
                key_down.at(key) = false;
            } catch (const std::out_of_range &) {}
        }

        constexpr bool is_down(const KeyType &key) const noexcept {
            try {
                return key_down.at(key);
            } catch (const std::out_of_range &) {
                return false;
            }
        }

        constexpr bool is_down_unchecked(const KeyType &key) const noexcept {
            return key_down[key];
        }

        constexpr bool is_up(const KeyType &key) const noexcept {
            try {
                return !key_down.at(key);
            } catch (const std::out_of_range &) {
                return true;
            }
        }

        constexpr bool is_up_unchecked(const KeyType &key) const noexcept {
            return !key_down[key];
        }
    };

/*Subclass of KeyMgr, but added records for key clicked
 * NOTICE : record() MUST be called before down() and up(); refresh() must be called after the key management*/
    template<typename KeyType_=SDL_Keycode>
    class KeyClickMgr : public KeyMgr<KeyType_> {
    protected:
        KEY_MGR_TYPEDEFS
        KeyMap key_prev, key_click, key_release;
    public:
        KeyType cur_click;

        explicit KeyClickMgr(const std::vector<KeyType> &init) : KeyMgr<KeyType>(init) {
            for (const auto &key: init) {
                key_prev.insert({key, false});
                key_click.insert({key, false});
                key_release.insert({key, false});
            }
        }

        virtual void record() {
            key_prev = this->key_down;
        }

        virtual void refresh() {
            for (const auto &key: key_prev) {
                auto &k_d = this->key_down.at(key.first);
                if (k_d ^ key.second) {
                    if (k_d)
                        cur_click = key.first;
                    key_click.at(key.first) = k_d;
                    key_release.at(key.first) = !k_d;
                    return;
                } else {
                    key_click.at(key.first) = false;
                    key_release.at(key.first) = false;
                }
            }
            cur_click = 0;
        }

        virtual void refresh_unchecked() noexcept {
            for (const auto &key: key_prev) {
                auto &k_d = this->key_down[key.first];
                if (k_d ^ key.second) {
                    cur_click = key.first;
                    key_click[key.first] = k_d;
                    key_release[key.first] = !k_d;
                    return;
                }
            }
        }

        bool is_click(KeyType key) const {
            try {
                return key_click.at(key);
            } catch (const std::out_of_range &) {
                return false;
            }
        }

        bool is_click_unchecked(KeyType key) const noexcept {
            return key_click[key];
        }

        bool is_release(KeyType key) const {
            try {
                return key_release.at(key);
            } catch (const std::out_of_range &) {
                return true;
            }
        }

        bool is_release_unchecked(KeyType key) const noexcept {
            return key_release[key];
        }
    };

    class MouseMgr : public MOUSE_CLICK_MGR_PARENT {
    protected:
    public:
        using MgrParent = MOUSE_CLICK_MGR_PARENT;
        using KeyType = MgrParent::KeyType;
        using KeyMap = MgrParent::KeyMap;
        using ButtonType = MOUSE_BUTTON_TYPE;
        std::unordered_map<ButtonType, Point> position;
        Point wheel_rel;

        static const int pos = 0, left = 1, middle = 2, right = 3, move = 4;

        MouseMgr() : MgrParent(MOUSE_KEYS) {
            for (auto key: MOUSE_KEYS)
                position.insert({key, VOID_POINT});
        }

        void record() override {
            MgrParent::record();
            wheel_rel.to0();
            for (auto &key: MOUSE_KEYS_EXCEPT_0)
                position.at(key).to0();
        }

        void motion(const SDL_MouseButtonEvent &button) {
            auto &prev = position.at(pos);
            position.at(move) = {button.x - prev.x, button.y - prev.y};
            prev = {button.x, button.y};
        }

        void wheel_motion(const SDL_MouseWheelEvent &wheel_event) noexcept {
            wheel_rel = {wheel_event.x, wheel_event.y};
        }

        void down(const SDL_MouseButtonEvent &button) noexcept {
            MgrParent::down(button.button);
            try {
                position.at(button.button) = {button.x, button.y};
            } catch (const std::out_of_range &) {}
        }

        void down_unchecked(const SDL_MouseButtonEvent &button) noexcept {
            MgrParent::down_unchecked(button.button);
            position[button.button] = {button.x, button.y};
        }

        void up(const SDL_MouseButtonEvent &button) noexcept {
            MgrParent::up(button.button);
            try {
                position.at(button.button) = VOID_POINT;
            } catch (const std::out_of_range &) {}
        }

        void up_unchecked(const SDL_MouseButtonEvent &button) noexcept {
            MgrParent::up_unchecked(button.button);
            position[button.button] = VOID_POINT;
        }


        [[nodiscard]] bool unmoved() const noexcept {
            return position.at(move).is0();
        }

        [[nodiscard]] bool moved() const noexcept {
            return position.at(move).isn0();
        }

        [[nodiscard]] bool wheel_unmoved() const noexcept {
            return wheel_rel.is0();
        }

        [[nodiscard]] bool wheel_moved() const noexcept {
            return wheel_rel.isn0();
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

    template<typename KeyType_ = SDL_Keycode>
    class MouseAndKeyClickMgr : public KeyClickMgr<KeyType_> {
    public:
        KEY_MGR_TYPEDEFS
        MouseMgr mouse;

        explicit MouseAndKeyClickMgr(const std::vector<KeyType> &init) : KeyClickMgr<KeyType_>(init) {}

        void record() override {
            KeyClickMgr<KeyType_>::record();
            mouse.record();
        }

        void refresh() override {
            KeyClickMgr<KeyType_>::refresh();
            mouse.refresh();
        }

        void refresh_unchecked() noexcept override {
            KeyClickMgr<KeyType_>::refresh_unchecked();
            mouse.refresh_unchecked();
        }
    };

NS_END

#undef KEY_MGR_TYPEDEFS

#undef MOUSE_BUTTON_TYPE
#undef MOUSE_CLICK_MGR_PARENT
#undef MOUSE_KEYS

#endif //SDLCLASS_EXTKEYMGR_HPP
