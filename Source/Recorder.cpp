#include "Recorder.h"

// Start recording
void Recorder::StartRecording() {
    this->is_recording_active = true;
    this->is_rerecording_active = false; // Ensure re-recording is not active
}

// Stop recording
void Recorder::StopRecording() {
    this->is_recording_active = false;
}

// Check if recording is active
bool Recorder::IsRecordingActive() const {
    return this->is_recording_active;
}

// Get the currently active recording
std::vector<Frame>& Recorder::GetActiveRecording() {
    return this->recording_frames;
}

// Start re-recording from a specific frame
void Recorder::StartRerecording(size_t start_frame) {
    this->is_rerecording_active = true;
    this->rerecording_start_frame = start_frame;
}

// Stop re-recording and optionally merge frames
void Recorder::StopRerecording(bool merge) {
    if (merge) {
        // Remove frames after the start frame and add re-recording frames
        this->recording_frames.erase(this->recording_frames.begin() + (this->rerecording_start_frame + 1), this->recording_frames.end());
        this->recording_frames.reserve(this->recording_frames.size() + this->rerecording_frames.size());
        this->recording_frames.insert(this->recording_frames.end(), this->rerecording_frames.begin(), this->rerecording_frames.end());
    }

    // Clear re-recording state
    this->is_rerecording_active = false;
    this->rerecording_start_frame = 0;
    this->rerecording_frames.clear();
}

// Check if re-recording is active
bool Recorder::IsRerecordingActive() const {
    return this->is_rerecording_active;
}

// Get the currently active re-recording
std::vector<Frame>& Recorder::GetActiveRerecording() {
    return this->rerecording_frames;
}