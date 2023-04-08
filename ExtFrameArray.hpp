//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTFRAMEARRAY_HPP
#define SDLCLASS_TEST_EXTFRAMEARRAY_HPP

#include "ExtBase.h"

NS_BEGIN

    /*An array of frames played continuously*/
    class FrameArray final {
    protected:
        using PointRef = const Point &;
        using FrameVector = std::vector<Surface *>;
        FrameVector *frames;
        size_t counter = 0, delay, index;
        int times = -1;
        bool independent, paused = false;

        explicit FrameArray(FrameVector *frames, size_t delay, size_t index) :
                frames(frames), delay(delay), index(index), independent(false) {}

    public:
        FrameArray(const FrameArray &) = delete;

        ~FrameArray() {
            if (independent) {
                for (auto &frame: *frames)
                    delete frame;
                delete frames;
            }
        }

        /*The frames are NOT managed in FrameArray(shallow copy)*/
        static FrameArray copy(FrameArray &src) {
            return FrameArray{src.frames, src.delay, src.index};
        }

        /*The frames ARE managed in FrameArray(deepcopy)*/
        static FrameArray deepcopy(FrameArray &src) {
            return FrameArray{*src.frames, src.delay, src.index};
        }

        /*The frames ARE managed in FrameArray(creation)*/
        static FrameArray
        from_dir(const std::filesystem::path &src, const std::string &suffix, size_t delay = 0, size_t index = 0) {
            FrameVector frames;
            for (const auto &entry: std::filesystem::directory_iterator(src)) {
                if (entry.is_regular_file()) {
                    auto path = entry.path().string();
                    if (path.ends_with(suffix))
                        frames.push_back(new Surface(entry.path().string()));
                }

            }
            return FrameArray{frames, delay, index};
        }

        /*The frames ARE managed in FrameArray(creation)*/
        explicit FrameArray(const FrameVector &frames, size_t delay = 1, size_t index = 0) :
                frames(new FrameVector(frames)), delay(delay), index(index), independent(true) {}

        bool next() noexcept {
            if ((counter++ % delay) == 0) {
                if (times == 0) return false;
                else if (times < 0) return true;
                else return --times;
            }
            return false;
        }

        /*Copy the current frame and tick once, return if the array hit the end of play times*/
        bool copy_to(RendererPtr renderer, PointRef pos) {
            bool next_ok = false;
            if (!paused && (next_ok = next())) index = (index + 1) % frames->size();
            frames->at(index)->copy_to(renderer, pos);
            return next_ok;
        }

        /*Set the play times, if -1, then play endlessly*/
        void set_times(int play_times = -1) {
            times = play_times;
        }

        void pause() {
            paused = true;
        }

        void unpause() {
            paused = false;
        }
    };
NS_END

#endif //SDLCLASS_TEST_EXTFRAMEARRAY_HPP
