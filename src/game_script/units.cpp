/*
 *	Author: Diego Ochando Torres
 *  Date: 09/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */


// ------------------------------------------------------------------------- //

#include "units.h"
#include "network_game.h"

// ------------------------------------------------------------------------- //

UnitManager::UnitManager(){

	p1_path_ = nullptr;
	p2_path_ = nullptr;

}

// ------------------------------------------------------------------------- //

UnitManager::~UnitManager(){

	delete p1_path_;
	delete p2_path_;

}

// ------------------------------------------------------------------------- //

void UnitManager::init(int client_id){

	client_id_ = client_id;

	initPaths();
	initUnits();

}

// ------------------------------------------------------------------------- //

void UnitManager::drawPaths(){

	p1_path_->drawDebug();
	p2_path_->drawDebug();

}

// ------------------------------------------------------------------------- //

Agent* UnitManager::createUnit(bool send_command, int client_id){

	Agent* agent = nullptr;

	if (client_id == 2) {
		agent = new Agent();
		agent->init(glm::vec3(520, 24, 0));
		agent->set_brake_force(1.0f);
		agent->set_agent_size(Agent::kAgentSize_Small);
		agent->set_movement_mode(Agent::kMovementMode_Deterministic);
		agent->set_deterministic_path(p2_path_);
		agent->client_owner_id_ = 2;
		agent->unit_id_ = agents_p2_.size();

		Sprite* p2_sprite = new Sprite(*agent, "../../../data/images/units/p2_unit1.png");
		agent->addComponent(p2_sprite);

		agents_p2_.push_back(agent);

		if (send_command) {
			UnitData* unit_data = CreateUnitData(agent->client_owner_id_, glm::vec2(520, 24),
				agent->unit_id_, 10, true);
			NetworkGame::instance().unit_manager_->updateUnit(true, *unit_data);
		}
	}
	else {
		agent = new Agent();
		agent->init(glm::vec3(88, 376, 0));
		agent->set_brake_force(1.0f);
		agent->set_agent_size(Agent::kAgentSize_Small);
		agent->set_movement_mode(Agent::kMovementMode_Deterministic);
		agent->set_deterministic_path(p1_path_);
		agent->client_owner_id_ = 1;
		agent->unit_id_ = agents_p1_.size();

		Sprite* p1_sprite = new Sprite(*agent, "../../../data/images/units/p1_unit1.png");
		agent->addComponent(p1_sprite);
		
		agents_p1_.push_back(agent);

		if (send_command) {
			UnitData* unit_data = CreateUnitData(agent->client_owner_id_, glm::vec2(88, 376), 
				agent->unit_id_, 10, true);
			NetworkGame::instance().unit_manager_->updateUnit(true, *unit_data);
		}
	}

	NetworkGame::instance().getScene()->addGameObject(agent);

	return agent;

}

// ------------------------------------------------------------------------- //

void UnitManager::updateUnit(bool send_command, UnitData& data){

	if (send_command) {
		// Add command to the send cmd list
		NetworkGame::instance().cmd_list_->commands_.push_back(&data);
	}
	else {
		// Move to position and store life, etc
		if (data.sender_id == 2) {
			for (int i = 0; i < agents_p2_.size(); ++i) {
				if (agents_p2_[i]->unit_id_ == data.id) {
					agents_p2_[i]->transform_.position_.x = data.x;
					agents_p2_[i]->transform_.position_.y = data.y;
					agents_p2_[i]->active_ = data.active;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < agents_p1_.size(); ++i) {
				if (agents_p1_[i]->unit_id_ == data.id) {
					agents_p1_[i]->transform_.position_.x = data.x;
					agents_p1_[i]->transform_.position_.y = data.y;
					agents_p1_[i]->active_ = data.active;
					break;
				}
			}
		}
	}

}

// ------------------------------------------------------------------------- //

void UnitManager::killUnit(bool send_command, UnitData& data){



}

// ------------------------------------------------------------------------- //

bool UnitManager::isUnitCreated(int player_id, int unit_id){

	if (player_id == 2) {
		for (int i = 0; i < agents_p2_.size(); ++i) {
			if (agents_p2_[i]->unit_id_ == unit_id) return true;
		}
	}
	else {
		for (int i = 0; i < agents_p1_.size(); ++i) {
			if (agents_p1_[i]->unit_id_ == unit_id) return true;
		}
	}

	return false;
	
}

// ------------------------------------------------------------------------- //

void UnitManager::initUnits(){

	if (client_id_ == 2) {
		createUnit(true, 2);
	}
	else {
		createUnit(true, 1);
	}

}

// ------------------------------------------------------------------------- //

void UnitManager::initPaths(){

	p1_path_ = new Path();
	p1_path_->addPoint(glm::vec2(88, 376));
	p1_path_->addPoint(glm::vec2(88, 360));
	p1_path_->addPoint(glm::vec2(104, 360));
	p1_path_->addPoint(glm::vec2(104, 344));
	p1_path_->addPoint(glm::vec2(120, 344));
	p1_path_->addPoint(glm::vec2(136, 344));
	p1_path_->addPoint(glm::vec2(152, 344));
	p1_path_->addPoint(glm::vec2(168, 344));
	p1_path_->addPoint(glm::vec2(184, 344));
	p1_path_->addPoint(glm::vec2(184, 360));
	p1_path_->addPoint(glm::vec2(200, 360));
	p1_path_->addPoint(glm::vec2(216, 360));
	p1_path_->addPoint(glm::vec2(232, 360));
	p1_path_->addPoint(glm::vec2(248, 360));
	p1_path_->addPoint(glm::vec2(264, 360));
	p1_path_->addPoint(glm::vec2(280, 360));
	p1_path_->addPoint(glm::vec2(296, 360));
	p1_path_->addPoint(glm::vec2(312, 360));
	p1_path_->addPoint(glm::vec2(328, 360));
	p1_path_->addPoint(glm::vec2(344, 360));
	p1_path_->addPoint(glm::vec2(360, 360));
	p1_path_->addPoint(glm::vec2(376, 360));
	p1_path_->addPoint(glm::vec2(392, 360));
	p1_path_->addPoint(glm::vec2(408, 360));
	p1_path_->addPoint(glm::vec2(424, 360));
	p1_path_->addPoint(glm::vec2(440, 360));
	p1_path_->addPoint(glm::vec2(456, 360));
	p1_path_->addPoint(glm::vec2(472, 360));
	p1_path_->addPoint(glm::vec2(488, 360));
	p1_path_->addPoint(glm::vec2(504, 360));
	p1_path_->addPoint(glm::vec2(504, 344));
	p1_path_->addPoint(glm::vec2(504, 328));
	p1_path_->addPoint(glm::vec2(504, 312));
	p1_path_->addPoint(glm::vec2(488, 312));
	p1_path_->addPoint(glm::vec2(488, 296));
	p1_path_->addPoint(glm::vec2(472, 296));
	p1_path_->addPoint(glm::vec2(456, 296));
	p1_path_->addPoint(glm::vec2(456, 280));
	p1_path_->addPoint(glm::vec2(440, 280));
	p1_path_->addPoint(glm::vec2(440, 264));
	p1_path_->addPoint(glm::vec2(440, 248));
	p1_path_->addPoint(glm::vec2(440, 232));
	p1_path_->addPoint(glm::vec2(456, 232));
	p1_path_->addPoint(glm::vec2(456, 216));
	p1_path_->addPoint(glm::vec2(472, 216));
	p1_path_->addPoint(glm::vec2(488, 216));
	p1_path_->addPoint(glm::vec2(504, 216));
	p1_path_->addPoint(glm::vec2(504, 200));
	p1_path_->addPoint(glm::vec2(520, 200));
	p1_path_->addPoint(glm::vec2(520, 184));
	p1_path_->addPoint(glm::vec2(536, 184));
	p1_path_->addPoint(glm::vec2(536, 168));
	p1_path_->addPoint(glm::vec2(536, 152));
	p1_path_->addPoint(glm::vec2(536, 136));
	p1_path_->addPoint(glm::vec2(536, 120));
	p1_path_->addPoint(glm::vec2(552, 120));
	p1_path_->addPoint(glm::vec2(552, 104));
	p1_path_->addPoint(glm::vec2(552, 88));
	p1_path_->addPoint(glm::vec2(552, 72));
	p1_path_->addPoint(glm::vec2(552, 56));
	p1_path_->set_ready();
	p1_path_->set_direction(Path::kDirection_Forward);
	p1_path_->set_action(Path::kAction_Straight);
	p1_path_->calculateTransformedPoints();

	p2_path_ = new Path();
	p2_path_->addPoint(glm::vec2(520, 24));
	p2_path_->addPoint(glm::vec2(520, 40));
	p2_path_->addPoint(glm::vec2(504, 40));
	p2_path_->addPoint(glm::vec2(504, 56));
	p2_path_->addPoint(glm::vec2(488, 56));
	p2_path_->addPoint(glm::vec2(472, 56));
	p2_path_->addPoint(glm::vec2(472, 40));
	p2_path_->addPoint(glm::vec2(456, 40));
	p2_path_->addPoint(glm::vec2(440, 40));
	p2_path_->addPoint(glm::vec2(440, 56));
	p2_path_->addPoint(glm::vec2(440, 56));
	p2_path_->addPoint(glm::vec2(424, 56));
	p2_path_->addPoint(glm::vec2(408, 56));
	p2_path_->addPoint(glm::vec2(392, 56));
	p2_path_->addPoint(glm::vec2(376, 56));
	p2_path_->addPoint(glm::vec2(376, 72));
	p2_path_->addPoint(glm::vec2(360, 72));
	p2_path_->addPoint(glm::vec2(344, 72));
	p2_path_->addPoint(glm::vec2(328, 72));
	p2_path_->addPoint(glm::vec2(312, 72));
	p2_path_->addPoint(glm::vec2(296, 72));
	p2_path_->addPoint(glm::vec2(280, 72));
	p2_path_->addPoint(glm::vec2(264, 72));
	p2_path_->addPoint(glm::vec2(248, 72));
	p2_path_->addPoint(glm::vec2(232, 72));
	p2_path_->addPoint(glm::vec2(216, 72));
	p2_path_->addPoint(glm::vec2(200, 72));
	p2_path_->addPoint(glm::vec2(184, 72));
	p2_path_->addPoint(glm::vec2(168, 72));
	p2_path_->addPoint(glm::vec2(152, 72));
	p2_path_->addPoint(glm::vec2(136, 72));
	p2_path_->addPoint(glm::vec2(120, 72));
	p2_path_->addPoint(glm::vec2(120, 88));
	p2_path_->addPoint(glm::vec2(104, 88));
	p2_path_->addPoint(glm::vec2(88, 88));
	p2_path_->addPoint(glm::vec2(88, 104));
	p2_path_->addPoint(glm::vec2(88, 120));
	p2_path_->addPoint(glm::vec2(88, 136));
	p2_path_->addPoint(glm::vec2(88, 152));
	p2_path_->addPoint(glm::vec2(104, 152));
	p2_path_->addPoint(glm::vec2(104, 168));
	p2_path_->addPoint(glm::vec2(120, 168));
	p2_path_->addPoint(glm::vec2(136, 168));
	p2_path_->addPoint(glm::vec2(136, 184));
	p2_path_->addPoint(glm::vec2(152, 184));
	p2_path_->addPoint(glm::vec2(152, 200));
	p2_path_->addPoint(glm::vec2(152, 216));
	p2_path_->addPoint(glm::vec2(152, 232));
	p2_path_->addPoint(glm::vec2(152, 248));
	p2_path_->addPoint(glm::vec2(136, 248));
	p2_path_->addPoint(glm::vec2(136, 264));
	p2_path_->addPoint(glm::vec2(120, 264));
	p2_path_->addPoint(glm::vec2(120, 280));
	p2_path_->addPoint(glm::vec2(104, 280));
	p2_path_->addPoint(glm::vec2(88, 280));
	p2_path_->addPoint(glm::vec2(72, 280));
	p2_path_->addPoint(glm::vec2(72, 296));
	p2_path_->addPoint(glm::vec2(56, 296));
	p2_path_->addPoint(glm::vec2(56, 312));
	p2_path_->addPoint(glm::vec2(56, 328));
	p2_path_->addPoint(glm::vec2(56, 344));
	p2_path_->set_ready();
	p2_path_->set_direction(Path::kDirection_Forward);
	p2_path_->set_action(Path::kAction_Straight);
	p2_path_->calculateTransformedPoints();

}

// ------------------------------------------------------------------------- //

