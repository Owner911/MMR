#include "renderer.h"
#include "Recorder.h"
#include "Playback.h"
#include "imgui.h"

// Declare global instances of Recorder and Playback
static Recorder recorder;
static Playback playback;

namespace DX9GenericHooks {
    // Replacement 'WndProc' function for intercepting all window messages.
    LRESULT STDMETHODCALLTYPE WndProc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param) {
        // Let the renderer decide whether we should pass this message to the game.
        if (!renderer.OnInputMessage(message_type, w_param, l_param))
            return true;

        return CallWindowProc(renderer.WndProc, window, message_type, w_param, l_param);
    };

    // Replacement 'Reset' function for re-creating invalid device objects.
    HRESULT STDMETHODCALLTYPE Reset(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        renderer.OnDeviceReset(false);

        HRESULT result = renderer.Reset(thisptr, params);

        ImGui_ImplDX9_CreateDeviceObjects();
        renderer.OnDeviceReset(true);

        return result;
    };

    // Replacement 'EndScene' function for custom rendering.
    HRESULT STDMETHODCALLTYPE EndScene(IDirect3DDevice9* thisptr) {
        ImGui_ImplDX9_NewFrame();
        renderer.OnEndScene();
        ImGui::Render();

        return renderer.EndScene(thisptr);
    };
};

// Restore the original WndProc on destruction.
DX9RenderMgr::~DX9RenderMgr() {
    SetWindowLongPtr(this->window, GWLP_WNDPROC, LONG_PTR(renderer.WndProc));
};

// Initialise and return the singleton instance.
inline DX9RenderMgr& DX9RenderMgr::GetInstance() {
    static DX9RenderMgr instance;
    return instance;
};

bool DX9RenderMgr::Initialize(IDirect3DDevice9* d3d9_device, const HWND& window) {
    // Hook 'Reset' and 'EndScene' from IDirect3DDevice9.
    this->d3d9_hook = std::make_unique<VMTHook>(d3d9_device);
    this->d3d9_hook->HookFunction(&DX9GenericHooks::Reset, 16);
    this->d3d9_hook->HookFunction(&DX9GenericHooks::EndScene, 42);

    // Store the original functions for later usage.
    this->Reset = this->d3d9_hook->GetOriginalFunction<Reset_t>(16);
    this->EndScene = this->d3d9_hook->GetOriginalFunction<EndScene_t>(42);

    // Store the window handle and device pointer as member variables.
    this->d3d9_device = d3d9_device;
    this->window = window;

    // Replace the WndProc and store the original as a member variable.
    renderer.WndProc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(DX9GenericHooks::WndProc))
        );

    if (ImGui_ImplDX9_Init(window, d3d9_device)) {
        renderer.OnInitialize();
        return true;
    }

    return false;
};

// Implementation of the OnEndScene method in Renderer
void Renderer::OnEndScene() {
    // Create a new ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui::NewFrame();

    // Create a new ImGui window
    ImGui::Begin("TAS Menu");

    // Check if recording is active and create appropriate buttons
    if (recorder.IsRecordingActive()) {
        if (ImGui::Button("Stop Recording")) {
            recorder.StopRecording();
        }
        if (ImGui::Button("Start Re-recording")) {
            // You may need to handle starting re-recording here
            // This button could be linked to some UI or keybinding
        }
    }
    else if (recorder.IsRerecordingActive()) {
        if (ImGui::Button("Save Re-recording")) {
            recorder.StopRerecording(true);
        }
        if (ImGui::Button("Clear Re-recording")) {
            recorder.StopRerecording(false);
        }
    }
    else {
        if (ImGui::Button("Start Recording")) {
            recorder.StartRecording();
        }
        if (!recorder.GetActiveRecording().empty()) {
            if (ImGui::Button("Clear Recorded Frames")) {
                recorder.GetActiveRecording().clear();
            }
        }
    }

    // Check if playback is active and create appropriate buttons
    if (playback.IsPlaybackActive()) {
        if (ImGui::Button("Stop Playback")) {
            playback.StopPlayback();
        }
    }
    else {
        if (!recorder.GetActiveRecording().empty()) {
            if (ImGui::Button("Start Playback")) {
                playback.StartPlayback(recorder.GetActiveRecording());
            }
        }
    }

    // End ImGui window
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
