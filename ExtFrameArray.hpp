//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTFRAMEARRAY_HPP
#define SDLCLASS_TEST_EXTFRAMEARRAY_HPP

#include "ExtBase.h"

NS_BEGIN

    /*An array of frames played continuously*/
    class FrameArray {
    protected:
        using PointRef = const Point &;
        using FrameVector = std::vector<Surface *>;
        FrameVector *frames;
        size_t counter = 0, delay, index;
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

        /*The frames ARE managed in FrameArray(deepcopy)*/
        explicit FrameArray(const FrameVector &frames, size_t delay = 1, size_t index = 0) :
                frames(new FrameVector(frames)), delay(delay), index(index), independent(true) {}

        constexpr bool next() noexcept {
            return (counter++ % delay) == 0;
        }

        void copy_to(RendererPtr renderer, PointRef pos) {
            if (!paused && next())index = (index + 1) % frames->size();
            frames->at(index)->copy_to(renderer, pos);
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