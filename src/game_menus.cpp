/*
 *	Author: Diego Ochando Torres
 *  Date: 10/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

 // ------------------------------------------------------------------------- //

#include "game_menus.h"
#include "network_game.h"

#include <imgui_sdl.h>
#include <imgui_impl_sdl.h>

// ------------------------------------------------------------------------- //

GameMenus::GameMenus() {



}

// ------------------------------------------------------------------------- //

GameMenus::~GameMenus() {



}

// ------------------------------------------------------------------------- //

void GameMenus::initGUI() {

	// Library things
	ImGui::CreateContext();
	ImGuiSDL::Initialize(NetworkGame::instance().renderer_, 608, 600);

	ImGuiStyle& style_ = ImGui::GetStyle();
	style_.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.5f, 0.0f, 1.00f);

	window_flags_ = 0;
	window_flags_ |= ImGuiWindowFlags_NoScrollbar;
	//window_flags_ |= ImGuiWindowFlags_MenuBar;
	window_flags_ |= ImGuiWindowFlags_NoMove;
	window_flags_ |= ImGuiWindowFlags_NoResize;
	window_flags_ |= ImGuiWindowFlags_NoCollapse;



	// Create menus
	createBasicMenu();
	createBuildingMenu();
	createUnitsMenu();

}

// ------------------------------------------------------------------------- //

void GameMenus::inputGUI(SDL_Event& event) {

	ImGuiIO& io = ImGui::GetIO();

	// Keyboard mapping. ImGui will use those indices to peek into
	// the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = SDLK_a;
	io.KeyMap[ImGuiKey_C] = SDLK_c;
	io.KeyMap[ImGuiKey_V] = SDLK_v;
	io.KeyMap[ImGuiKey_X] = SDLK_x;
	io.KeyMap[ImGuiKey_Y] = SDLK_y;
	io.KeyMap[ImGuiKey_Z] = SDLK_z;

	int mouseX = 0, mouseY = 0, wheel = 0;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	switch (event.type) {
	case SDL_TEXTINPUT: {
		io.AddInputCharactersUTF8(event.text.text);
		break;
	}
	case SDL_KEYUP: {
		int key = event.key.keysym.sym & ~SDLK_SCANCODE_MASK;
		io.KeysDown[key] = (event.type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		break;
	}
	}

}

// ------------------------------------------------------------------------- //

void GameMenus::manageGUI() {

	ImGui::NewFrame();

	//Things to draw
	//ImGui::ShowDemoWindow();
	//...
	//...

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 400), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(608, 200), ImGuiCond_Always);

	static bool p_open = true;
	ImGui::Begin("Menu", &p_open, window_flags_);

	ImGui::End();

}

// ------------------------------------------------------------------------- //

void GameMenus::drawGUI() {

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());

}

// ------------------------------------------------------------------------- //

void GameMenus::shutdownGUI() {

	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();

}

// ------------------------------------------------------------------------- //

void GameMenus::createBasicMenu() {



}

// ------------------------------------------------------------------------- //

void GameMenus::createBuildingMenu() {



}

// ------------------------------------------------------------------------- //

void GameMenus::createUnitsMenu() {



}

// ------------------------------------------------------------------------- //