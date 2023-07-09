//
// Created by Dogs-Cute on 4/5/2023.
//

#ifndef SDLCLASS_TEST_EXTFRAMEARRAY_HPP
#define SDLCLASS_TEST_EXTFRAMEARRAY_HPP

#include <utility>

#include "ExtBase.h"

NS_BEGIN

    /*An array of frames played continuously*/
    class FrameArray final {
    protected:
        using PointRef = const Point &;
        using FrameVector = std::shared_ptr<std::vector<std::shared_ptr<Surface>>>;
        using SurfaceProcessor = std::function<void(const std::shared_ptr<Surface> &)>;
        FrameVector frames;
        size_t counter = 0, delay, index;
        int times = -1;
        bool paused = false;

        static FrameVector frames_from_dir(const std::filesystem::path &src, const std::string &suffix,
                                           const SurfaceProcessor *processor = nullptr) {
            FrameVector frames = std::make_shared<std::vector<std::shared_ptr<Surface>>>();
            size_t count = 0;
            for (const auto &entry: std::filesystem::directory_iterator(src)) {
                if (entry.path().extension() == suffix) {
                    auto surface = std::make_shared<Surface>(entry.path().string());
                    if (processor)
                        (*processor)(surface);
                    frames->push_back(surface);
                    count++;
                }
            }
            if (count == 0)
                throw std::runtime_error("No frames available found in directory: " + src.string());
            return frames;
        }

    public:
        ~FrameArray() = default;

        [[nodiscard]] FrameArray deepcopy() const {
            FrameVector new_frames = std::make_shared<std::vector<std::shared_ptr<Surface>>>(*frames);
            return FrameArray{new_frames, delay, index};
        }

        static FrameArray
        from_dir(const std::filesystem::path &src, const std::string &suffix, size_t delay = 1, size_t index = 0) {
            return FrameArray{frames_from_dir(src, suffix), delay, index};
        }

        static std::shared_ptr<FrameArray>
        ptr_from_dir(const std::filesystem::path &src, const std::string &suffix, size_t delay = 1, size_t index = 0) {
            return std::make_shared<FrameArray>(frames_from_dir(src, suffix), delay, index);
        }

        static FrameArray
        from_dir(const std::filesystem::path &src, const std::string &suffix, const SurfaceProcessor &processor,
                 size_t delay = 1, size_t index = 0) {
            return FrameArray{frames_from_dir(src, suffix, &processor), delay, index};
        }

        static std::shared_ptr<FrameArray>
        ptr_from_dir(const std::filesystem::path &src, const std::string &suffix, const SurfaceProcessor &processor,
                     size_t delay = 1, size_t index = 0) {
            return std::make_shared<FrameArray>(frames_from_dir(src, suffix, &processor), delay, index);
        }

        explicit FrameArray(FrameVector frames, size_t delay = 1, size_t index = 0) :
                frames(std::move(frames)), delay(delay), index(index) {
            if (delay == 0)
                throw std::invalid_argument("FrameArray::delay cannot be 0.");
        }

        bool next() noexcept {
            if ((counter++ % delay) == 0) {
                if (times == 0) return false;
                else if (times < 0) return true;
                else return --times;
            }
            return false;
        }

        template<CanBePointOrRect CopyToType>
        /*Copy the current frame and tick once, return if the array hit the end of play times*/
        bool copy_to(Renderer &renderer, const CopyToType &pos) {
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

        [[nodiscard]] Point size() const {
            return {frames->front()->w(), frames->front()->h()};
        }
    };
NS_END

#endif //SDLCLASS_TEST_EXTFRAMEARRAY_HPP
