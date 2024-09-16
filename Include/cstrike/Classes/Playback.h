#pragma once

#include <vector>
#include "Frame.h"

class Playback {
private:
    bool is_playback_active = false;
    size_t current_frame = 0;
    std::vector<Frame> active_demo;
public:
    void StartPlayback(std::vector<Frame>& frames);
    void StopPlayback();
    bool IsPlaybackActive() const;
    size_t GetCurrentFrame() const;
    void SetCurrentFrame(size_t frame);
    std::vector<Frame>& GetActiveDemo() const;
};