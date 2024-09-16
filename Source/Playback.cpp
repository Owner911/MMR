#include "Playback.h"

void Playback::StartPlayback(std::vector<Frame>& frames) {
    this->is_playback_active = true;
    this->active_demo = frames;
}

void Playback::StopPlayback() {
    this->is_playback_active = false;
    this->current_frame = 0;
}

bool Playback::IsPlaybackActive() const {
    return this->is_playback_active;
}

size_t Playback::GetCurrentFrame() const {
    return this->current_frame;
}

void Playback::SetCurrentFrame(size_t frame) {
    this->current_frame = frame;
}

std::vector<Frame>& Playback::GetActiveDemo() const {
    return this->active_demo;
}