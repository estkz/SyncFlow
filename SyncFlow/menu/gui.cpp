// Menu Related Includes
#include "../menu/gui.h"
#include "../memory/globals.h"
#include "icons.cpp"
#include "icons.h"

// ImGui DirectX9 Related Includes
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// Miscellaneous Related Includes
#include <string>
#include <vector>

// Here I store the width and height of our mnonitor in screenWidth and screenHeight
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

// To center the menu on the screen, we subtract the width and height of our monitor with the width and height of our menu. We then divide both with 2 and store windowX and windowY
int windowX = (screenWidth - gui::WIDTH) / 2;
int windowY = (screenHeight - gui::HEIGHT) / 2;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

long __stdcall WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
	{
		if (gui::device && wParam != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(lParam);
			gui::presentParameters.BackBufferHeight = HIWORD(lParam);
			gui::ResetDevice();
		}
	} return 0;



	case WM_SYSCOMMAND:
	{
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	} break;



	case WM_DESTROY:
	{
		PostQuitMessage(0);
	} return 0;



	case WM_LBUTTONDOWN:
	{
		gui::position = MAKEPOINTS(lParam); // Update the position variable with the coordinates where a click occurs.
	} return 0;



	case WM_MOUSEMOVE: {
		if (wParam == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(lParam);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			// Modify this logic if you wish to change the draggable area.
			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 100)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "SyncFlow";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowExA(0, "SyncFlow", windowName, WS_POPUP, windowX, windowY, WIDTH, HEIGHT, 0, 0, windowClass.hInstance, 0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

inline ImFont* icons_font = nullptr;

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	io.IniFilename = NULL;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14.0f);

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	static const ImWchar icon_ranges[]{ 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	// Avoid conflict with other fonts.
	icons_config.MergeMode = true;

	// Make Icons Cleaner
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	// Function call, which adds the FontAwesome icons and making it available for rendering in ImGui.
	icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 19.5f, &icons_config, icon_ranges);

	// Menu - Color Theme Config
	colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20); // Frame Backcolor
	colors[ImGuiCol_ChildBg] = ImColor(17, 17, 17); // Childform Backcolor

	// Button
	colors[ImGuiCol_Button] = ImColor(17, 17, 17); // Button Backcolor
	colors[ImGuiCol_ButtonActive] = ImColor(17, 17, 17); // Button Active
	colors[ImGuiCol_ButtonHovered] = ImColor(17, 17, 17); // Hover Color

	// Checkbox
	colors[ImGuiCol_FrameBg] = ImColor(50, 50, 50); // Checkbox Inside Color
	colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50); // Checkbox Inside Hover Color
	colors[ImGuiCol_FrameBgActive] = ImColor(0, 150, 109); // Checked Color
	colors[ImGuiCol_CheckMark] = ImColor(0, 150, 109);

	// Slider
	colors[ImGuiCol_SliderGrab] = ImColor(0, 150, 109);

	// Adjust Padding and Margins
	style.WindowPadding = ImVec2(0.0f, 0.0f);
	style.FramePadding = ImVec2(0.0f, 0.0f);

}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isActive = !isActive;
			return;
		}
	}

	// Start of the ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}



/*
	[+] ---------- Menu Customization ---------- [+]

				  • Component Styling
				  • Render Function

	[+] ---------------------------------------- [+]
*/

inline void PageButtons(std::vector<std::string> names, std::vector<int> indexes, int& selected_index)
{
	std::vector<ImVec2> sizes = {};
	float total_area = 0.0f;
	const auto& style = ImGui::GetStyle();

	for (std::string& name : names) {
		const ImVec2 label_size = ImGui::CalcTextSize(name.c_str(), 0, true);

		// Determine the maximum height needed for both the label and the icon
		float max_height = ImGui::GetTextLineHeightWithSpacing() + style.FramePadding.y * 2.0f;
		if (19.5f + style.FramePadding.y * 2.0f > max_height)
			max_height = 19.5f + style.FramePadding.y * 2.0f;

		// Calculate the size of the button with additional padding
		ImVec2 size = ImGui::CalcItemSize(
			ImVec2(),
			label_size.x + style.FramePadding.x * 2.0f + 60.0f, // Increase the width to accommodate the icon
			max_height + 20.0f   // Increase the height to accommodate the icon and provide extra padding
		);

		size.x += 45.5f;
		size.y += 15.f;

		sizes.push_back(size);
		total_area += size.x;
	}

	// Calculate maximum Y position to clamp
	float max_y_position = ImGui::GetCursorPosY() + 70;

	// Calculate the starting position for the buttons to move them to the right
	float start_x_position = (ImGui::GetContentRegionAvail().x / 1.8) - (total_area / 2);
	start_x_position += 100.0f; // Adjust this value as needed for the desired offset

	ImGui::SameLine(start_x_position);
	for (uint32_t i = 0; i < names.size(); i++)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

		if (selected_index == indexes[i])
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0, 150, 109).Value);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(0, 150, 109).Value);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0, 150, 109).Value);

			if (ImGui::Button(names[i].c_str(), sizes[i]))
				selected_index = indexes[i];
			ImGui::PopStyleColor(3);
		}
		else
		{
			if (ImGui::Button(names[i].c_str(), sizes[i]))
				selected_index = indexes[i];
		}

		ImGui::PopStyleVar();

		if (i != names.size() - 1) // Check if not the last button
			ImGui::SameLine();

		// Clamp the Y position of the buttons
		ImVec2 current_pos = ImGui::GetCursorPos();
		if (current_pos.y > max_y_position)
			current_pos.y = max_y_position;
		ImGui::SetCursorPos(current_pos);
	}
}

void gui::Render() noexcept
{
	static int index = 0;
	static int iAimbot = 0;
	static int iVisuals = 0;
	static int iTrainer = 0;

	// Set up ImGui window
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"SyncFlow",
		&isActive,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar
	);

	// Get ImGui style
	ImGuiStyle& style = ImGui::GetStyle();

	// Set item spacing to zero
	style.ItemSpacing = ImVec2(0.0f, 0.0f);

	// Page Selection
	ImGui::BeginChild("##selection_page", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 6.5));
	ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 15, ImGui::GetContentRegionAvail().y / 2.75));
	ImGui::SetWindowFontScale(2.0f);
	ImGui::Text("SYNC");
	ImGui::SameLine(113.0f);
	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored(ImVec4(0.0f / 255.0f, 150.0f / 255.0f, 109.0f / 255.0f, 1.0f), "FLOW");

	PageButtons({ ICON_FA_CROSSHAIRS, ICON_FA_EYE, ICON_FA_BUG }, { 0, 1, 2 }, index);
	ImGui::EndChild();
	// Page Selection End

	// Tab Selection
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0863f, 0.0863f, 0.0863f, 1.0f));
	ImGui::BeginChild("##selection_tab", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 7.5));

	if (index == 0)
	{
		ImGui::SetCursorPos(ImVec2(20, 20));
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("Aimbot");

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 1.62, ImGui::GetContentRegionAvail().y / 2));
		if (ImGui::Button("Aimbot", ImVec2(130, 45)))
		{
			iAimbot = 0;
			iVisuals = 0;
			iTrainer = 0;
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 1.25, ImGui::GetContentRegionAvail().y / .83));
		if (ImGui::Button("Triggerbot", ImVec2(130, 45)))
		{
			iAimbot = 1;
			iVisuals = 0;
			iTrainer = 0;
		}
	}
	else if (index == 1)
	{
		ImGui::SetCursorPos(ImVec2(20, 20));
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("Visuals");

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 1.62, ImGui::GetContentRegionAvail().y / 2));
		if (ImGui::Button("Enemy", ImVec2(130, 45)))
		{
			iAimbot = 0;
			iVisuals = 0;
			iTrainer = 0;
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 1.25, ImGui::GetContentRegionAvail().y / .83));
		if (ImGui::Button("World", ImVec2(130, 45)))
		{
			iAimbot = 0;
			iVisuals = 1;
			iTrainer = 0;
		}
	}
	else if (index == 2)
	{
		ImGui::SetCursorPos(ImVec2(20, 20));
		ImGui::SetWindowFontScale(1.5f);
		ImGui::Text("Trainer");

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 1.62, ImGui::GetContentRegionAvail().y / 2));
		if (ImGui::Button("Modifications", ImVec2(130, 45)))
		{
			iAimbot = 0;
			iVisuals = 0;
			iTrainer = 0;
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 1.25, ImGui::GetContentRegionAvail().y / .83));
		if (ImGui::Button("Exploits", ImVec2(130, 45)))
		{
			iAimbot = 0;
			iVisuals = 0;
			iTrainer = 1;
		}
	}
	else
	{
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::End();
	}

	ImGui::SetCursorPos(ImVec2(20, 45));
	ImGui::SetWindowFontScale(1.2f);
	ImGui::TextColored(ImColor(96, 96, 96).Value, "Select a tab:");
	ImGui::EndChild();
	ImGui::PopStyleColor();
	// Tab Selection End

	// Main Panel
	ImGui::BeginChild("##main_panel", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 1.11));

	if (index == 0)
	{
		if (iAimbot == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild("##aimbot_tab", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Aimbot");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);

			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Enabled", &globals::aimbotEnabled);
			ImGui::SetCursorPos(ImVec2(10, 70));
			ImGui::SliderFloat("FOV", &globals::aimFOV, 0.0f, 180.0f, "%.0f deg");

			ImGui::SetCursorPos(ImVec2(10, 90));
			ImGui::SliderFloat("Smoothing", &globals::aimSmoothing, 0.0f, 100.0f, "%.0f %%");

			ImGui::SetCursorPos(ImVec2(10, 110));
			ImGui::Checkbox("Aim Through Walls", &globals::aimThroughWalls);

			ImGui::EndChild();

			ImGui::SetCursorPos(ImVec2(385, 10));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::BeginChild("##aimbot_tab_extra", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Extra");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);

			ImGui::SetCursorPos(ImVec2(10, 50));	
			ImGui::Checkbox("No Recoil", &globals::noRecoil);

			ImGui::SetCursorPos(ImVec2(10, 70));
			ImGui::Checkbox("No Pushback", &globals::noPushback);

			ImGui::EndChild();
		}
		else if (iAimbot == 1)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild("##triggerbot_tab", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Triggerbot");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);

			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Enable", &globals::triggerEnabled);

			ImGui::SetCursorPos(ImVec2(10, 70));
			ImGui::PushItemWidth(200.0f);
			ImGui::SliderInt("Reaction Time", &globals::reactionTime, 0, 300, "%d ms");
			ImGui::PopItemWidth();

			ImGui::EndChild();
		}
	}
	else if (index == 1)
	{
		if (iVisuals == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild("##enemy_tab", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Enemy ESP");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);
			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Enable", &globals::espEnabled);

			ImGui::SetCursorPos(ImVec2(10, 70));
			ImGui::Checkbox("Bounding Box", &globals::boundingBox);

			ImGui::SetCursorPos(ImVec2(10, 90));
			ImGui::Checkbox("Health", &globals::healthBar);

			ImGui::SetCursorPos(ImVec2(10, 110));
			ImGui::Checkbox("Name", &globals::nameESP);

			ImGui::SetCursorPos(ImVec2(10, 130));
			ImGui::Checkbox("Weapon", &globals::weapon);

			ImGui::SetCursorPos(ImVec2(10, 150));
			ImGui::Checkbox("Distance", &globals::distance);

			ImGui::EndChild();

			ImGui::SetCursorPos(ImVec2(385, 10));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::BeginChild("##aimbot_tab_extra", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Extra");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);
			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Snaplines", &globals::snaplines);
		}
		else if (iVisuals == 1)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild("##triggerbot_tab", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("World ESP");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);

			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("World", &globals::nadeESP);

			ImGui::SetCursorPos(ImVec2(10, 70));
			ImGui::Checkbox("Flags", &globals::flagESP);
		}
	}
	else if (index == 2)
	{
		if (iTrainer == 0)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild("##trainer_tab", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Modifications");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);
			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Godmode", &globals::godmode);

			ImGui::SetCursorPos(ImVec2(10, 70));
			ImGui::Checkbox("Armor", &globals::armor);

			ImGui::SetCursorPos(ImVec2(10, 90));
			ImGui::Checkbox("Ammunition", &globals::ammo);

			ImGui::SetCursorPos(ImVec2(10, 110));
			ImGui::Checkbox("Nades", &globals::nades);

			ImGui::EndChild();

			ImGui::SetCursorPos(ImVec2(385, 10));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::BeginChild("##trainer_tab_misc", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Misc");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);
			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Bunnyhop", &globals::bunnyhop);
		}
		else if (iTrainer == 1)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(19, 19, 19, 255).Value);
			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::BeginChild("##exploits_tab", ImVec2(ImGui::GetContentRegionAvail().x - 10, ImGui::GetContentRegionAvail().y - 10));
			ImGui::PopStyleColor(1);

			ImGui::SetCursorPos(ImVec2(10, 10));
			ImGui::Text("Exploits");

			ImGui::Dummy(ImVec2(10, 10));
			ImGui::SeparatorEx(1.0f);

			ImGui::SetCursorPos(ImVec2(10, 50));
			ImGui::Checkbox("Rapidfire", &globals::rapidfire);
		}
	}

	ImGui::EndChild();
	// Main Panel End

	ImGui::End();
}