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


	//Load resources
	int width, height;
	if (NetworkGame::instance().client_id_ == 2) {
		tower_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p2_tower.png", &width, &height);
	
		house_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p2_house.png", &width, &height);

		farm_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p2_farm.png", &width, &height);

		woodhouse_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p2_wood.png", &width, &height);
	}
	else {
		tower_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p1_tower.png", &width, &height);
	
		house_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p1_house.png", &width, &height);

		farm_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p1_farm.png", &width, &height);

		woodhouse_texture_ = NetworkGame::instance().tex_loader_->loadTexture("../../../data/images/buildings/p1_wood.png", &width, &height);
	}



	debug_mode_ = false;
	build_mode_ = false;

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

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 400), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(608, 200), ImGuiCond_Always);

	static bool p_open = true;
	ImGui::Begin("Menu", &p_open, window_flags_);


	// Selected menu
	if (!NetworkGame::instance().game_started_) {
		ImGui::Text("Waiting for another player to join the game...");
	}
	else {
		if (build_mode_) {
			drawBuildingMenu();
		}
		else {
			drawBasicMenu();
		}
	}

	// Resources
	ImGui::SetCursorPos(ImVec2(370, 180));
	ImGui::Text("People: %d", NetworkGame::instance().build_manager_->people_pieces_);
	ImGui::SetCursorPos(ImVec2(460, 180));
	ImGui::Text("Food: %d", NetworkGame::instance().build_manager_->food_pieces_);
	ImGui::SetCursorPos(ImVec2(530, 180));
	ImGui::Text("Wood: %d", NetworkGame::instance().build_manager_->wood_pieces_);


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

void GameMenus::drawBasicMenu() {

	if (ImGui::Button("Buildings", ImVec2(80, 30))) {
		build_mode_ = true;
		NetworkGame::instance().build_manager_->mouse_build_object_->active_ = true;
	}
	if (ImGui::Button("Units", ImVec2(80, 30))) {

	}

}

// ------------------------------------------------------------------------- //

void GameMenus::drawBuildingMenu() {

	if (ImGui::ImageButton(tower_texture_, ImVec2(100, 100))) {
		NetworkGame::instance().build_manager_->selectBuilding(kBuildKind_DefenseTower);
	}
	ImGui::Text("Defense Tower");

	ImGui::SetCursorPos(ImVec2(120, 27));
	if (ImGui::ImageButton(house_texture_, ImVec2(100, 100))) {
		NetworkGame::instance().build_manager_->selectBuilding(kBuildKind_House);
	}
	ImGui::SetCursorPos(ImVec2(120, 137));
	ImGui::Text("House");

	ImGui::SetCursorPos(ImVec2(232, 27));
	if (ImGui::ImageButton(farm_texture_, ImVec2(100, 100))) {
		NetworkGame::instance().build_manager_->selectBuilding(kBuildKind_Farm);
	}
	ImGui::SetCursorPos(ImVec2(232, 137));
	ImGui::Text("Farm");

	ImGui::SetCursorPos(ImVec2(344, 27));
	if (ImGui::ImageButton(woodhouse_texture_, ImVec2(100, 100))) {
		NetworkGame::instance().build_manager_->selectBuilding(kBuildKind_WoodHouse);
	}
	ImGui::SetCursorPos(ImVec2(344, 137));
	ImGui::Text("Wood house");

	ImGui::SetCursorPosY(170);
	if (ImGui::Button("Back", ImVec2(40, 25))) {
		build_mode_ = false;
		NetworkGame::instance().build_manager_->mouse_build_object_->active_ = false;
	}

}

// ------------------------------------------------------------------------- //

void GameMenus::drawUnitsMenu() {



}

// ------------------------------------------------------------------------- //