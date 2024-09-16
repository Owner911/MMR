#include "Interface.h"

// Draw the user interface here!
void BasehookInterface::OnEndScene() {
	if (!is_menu_visible)
		return;

	ImGui::Text("Hello world!");
}

// Check for key presses here in order to toggle menu visibility.
bool BasehookInterface::OnInputMessage(UINT type, WPARAM w_param, LPARAM l_param) {
	// Listen for INSERT key presses.
	if (type == WM_KEYUP && w_param == VK_INSERT) {
		// Toggle menu visibility.
		is_menu_visible = !is_menu_visible;

		// Use the ImGui cursor.
		ImGui::GetIO().MouseDrawCursor = is_menu_visible;
		
		// Enable or disable the in-game cursor to prevent issues.
		matsurface->SetCursorAlwaysVisible(is_menu_visible);
	}

	// Pass input to ImGui if the menu is visible.
	if (is_menu_visible)
		ImGui_ImplDX9_WndProcHandler(window, type, w_param, l_param);

	// Pass input to game if the menu is not visible.
	return !is_menu_visible;
}