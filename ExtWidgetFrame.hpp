//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTWIDGETFRAME_HPP
#define SDLCLASS_TEST_EXTWIDGETFRAME_HPP

#include "ExtBase.h"
#include "ExtWidget.hpp"

NS_BEGIN
/*A frame containing one surface*/
    WIDGET_TEMPLATE(=KeyMgr<>)
    class Frame : public WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        Surface surface;
    public:
        Frame(PointRef pos, SDLSurfacePtr surface, bool independent = true) : WidgetParent{pos},
                                                                              surface(surface, independent) {}

        ~Frame() override = default;

        void set_surface(SDLSurfacePtr new_surface, bool independent = true) {
            surface = {new_surface, independent};
        }

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_frame);
        }

        WIDGET_PRESENT override {
            surface.copy_to(renderer, this->pos + rel);
        };

        WIDGET_TYPE(WidgetResult::t_frame)
    };

/*An array of frames that can repeatedly show them*/
    WIDGET_TEMPLATE(=KeyMgr<>)
    class FrameArray : public WidgetBase<MgrType> {
    protected:
        WIDGET_TYPEDEFS
        std::vector<Frame<MgrType> *> *frames;
        bool independent = true, paused = false;
        size_t index, counter = 0, counter_max;
    public:
        /**
         * \param frames the frames to initialize with
         * \param starting_index the index of frames when it first starts. Defaults to 0
         * \param counter_max this delays the frame switching. Defaults to 1*/
        explicit FrameArray(const std::vector<Frame<MgrType> *> &frames, size_t starting_index = 0,
                            size_t counter_max = 1) :
                WidgetParent{(*frames.begin())->pos}, frames(new std::vector<Frame<MgrType> *>(frames)),
                index(starting_index), counter_max(counter_max) {

        }

        /**
         * \param pos the position of the surfaces and FrameArray itself
         * \param frames the frames(\c SDLSurfacePtr) to initialize with
         * \param starting_index the index of frames when it first starts. Defaults to 0
         * \param counter_max this delays the frame switching. Defaults to 1*/
        FrameArray(PointRef pos, const std::vector<SDLSurfacePtr> &frames, size_t starting_index = 0,
                   size_t counter_max = 1) :
                WidgetParent{pos}, frames(new std::vector<Frame<MgrType> *>()), index(starting_index),
                counter_max(counter_max) {
            for (auto frame: frames)
                this->frames->push_back(new Frame<MgrType>{DEFAULT_POINT, frame, true});
        }

        FrameArray(const FrameArray<MgrType> &frame_array) : frames(frame_array.frames), independent(false),
                                                             index(index), counter_max(frame_array.counter_max) {}

        ~FrameArray() override {
            if (independent) {
                for (auto &frame: *frames)
                    delete frame;
                delete frames;
            }
        }

        WIDGET_PROCESS override {
            result.set_type(WidgetResult::t_frame_array);
            if ((counter++ % counter_max) == 0)
                ++index %= frames->size();
        };

        WIDGET_PRESENT override {
            frames->at(index)->present(renderer, rel);
        };

        WIDGET_TYPE(WidgetResult::t_frame_array)
    };
NS_END

#endif //SDLCLASS_TEST_EXTWIDGETFRAME_HPP
