#pragma once

#include <cstdint>

struct CUserCmd; // Forward declaration, include the proper header if needed

struct Frame {
    float viewangles[2];
    float forwardmove;
    float sidemove;
    float upmove;
    int buttons;
    unsigned char impulse;
    short mousedx;
    short mousedy;

    Frame(CUserCmd* cmd);
    void Replay(CUserCmd* cmd);
};

