#include "Frame.h"
#include "CUserCmd.h" // Include the header where CUserCmd is defined

Frame::Frame(CUserCmd* cmd) {
    this->viewangles[0] = cmd->viewangles.X;
    this->viewangles[1] = cmd->viewangles.Y;
    this->forwardmove = cmd->forwardmove;
    this->sidemove = cmd->sidemove;
    this->upmove = cmd->upmove;
    this->buttons = cmd->buttons;
    this->impulse = cmd->impulse;
    this->mousedx = cmd->mousedx;
    this->mousedy = cmd->mousedy;
}

void Frame::Replay(CUserCmd* cmd) {
    cmd->viewangles.X = this->viewangles[0];
    cmd->viewangles.Y = this->viewangles[1];
    cmd->forwardmove = this->forwardmove;
    cmd->sidemove = this->sidemove;
    cmd->upmove = this->upmove;
    cmd->buttons = this->buttons;
    cmd->impulse = this->impulse;
    cmd->mousedx = this->mousedx;
    cmd->mousedy = this->mousedy;
}