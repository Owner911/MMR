#include "Hooks.h"
#include "Recorder.h"
#include "Playback.h"

// Static instances of Recorder and Playback
static Recorder recorder;
static Playback playback;

bool __fastcall Hooks::CreateMove(ClientModeShared* thisptr, void* edx, float frametime, CUserCmd* command) {
    // Don't do anything when called from CInput::ExtraMouseSample.
    if (!command->command_number)
        return false;

    // Check if playback or recording is active
    bool is_playback_active = playback.IsPlaybackActive();
    bool is_recording_active = recorder.IsRecordingActive();
    bool is_rerecording_active = recorder.IsRerecordingActive();

    // Access the current recording frames
    std::vector<Frame>& recording = recorder.GetActiveRecording();
    std::vector<Frame>& rerecording = recorder.GetActiveRerecording();

    // Handle recording
    if (is_recording_active) {
        recording.push_back(Frame(command));
    }

    // Handle re-recording
    if (is_rerecording_active) {
        rerecording.push_back(Frame(command));
    }

    // Handle playback
    if (is_playback_active) {
        const size_t current_playback_frame = playback.GetCurrentFrame();

        try {
            // Replay the recorded input
            playback.GetActiveDemo().at(current_playback_frame).Replay(command);

            // Set the view angles for the engine (if needed)
            if (engine) {
                engine->SetViewAngles(command->viewangles);
            }

            // Check if the 're-record' button is pressed (assuming it’s mapped to any non-zero button input)
            if (command->buttons != 0) {
                recorder.StartRerecording(current_playback_frame);
                playback.StopPlayback();
                return false;
            }

            // Move to the next frame or stop playback if at the end
            if (current_playback_frame + 1 == playback.GetActiveDemo().size()) {
                playback.StopPlayback();
            }
            else {
                playback.SetCurrentFrame(current_playback_frame + 1);
            }
        }
        catch (std::out_of_range&) {
            // Stop playback if frame index is out of range
            playback.StopPlayback();
        }
    }

    // Return true to allow further processing or false if you want to stop it.
    return true;
}
