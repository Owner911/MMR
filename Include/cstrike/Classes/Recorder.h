#pragma once

#include <vector>
#include "Frame.h"

class Recorder {
private:
    bool is_recording_active = false;
    bool is_rerecording_active = false;

    size_t rerecording_start_frame;

    std::vector<Frame> recording_frames;
    std::vector<Frame> rerecording_frames;

public:
    // Methods for recording
    void StartRecording();
    void StopRecording();
    bool IsRecordingActive() const;
    std::vector<Frame>& GetActiveRecording();

    // Methods for re-recording
    void StartRerecording(size_t start_frame);
    void StopRerecording(bool merge = false);
    bool IsRerecordingActive() const;
    std::vector<Frame>& GetActiveRerecording();
};