/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include "agent.h"
#include "network_game.h"

#include <glm.hpp>

// ------------------------------------------------------------------------- //

Agent::Agent(){

  direction_ = glm::vec2(0.0f, 0.0f);
  desired_direction_ = glm::vec2(0.0f, 0.0f);

  acceleration_ = 0.0f;
  max_acceleration_ = 5.0f;

  target_ = glm::vec2(0.0f, 0.0f);
  reached_target_ = false; 

  movement_mode_ = kMovementMode_NONE;
  agent_size_ = kAgentSize_NONE;

  is_aproacching_target_ = false;
  brake_force_ = 0.3f;

  started_movement_ = false;

  current_pattern_ = 0;

  current_path_ = nullptr;
  refresh_time_ = 3.0f;
  accum_time_ = refresh_time_;

  fsm_state_ = kFSMState_Working;
  is_transitioning_ = false;

  client_owner_id_ = 0;
  unit_id_ = 0;

}

// ------------------------------------------------------------------------- //

Agent::~Agent(){

  

}

// ------------------------------------------------------------------------- //

void Agent::update(uint32_t time_step){

  GameObject::update(time_step);
  if(current_path_ != nullptr)
    current_path_->calculateTransformedPoints();
  
  current_box_.calculateTransformedPoints();

  if (accum_time_ >= refresh_time_) {
    updateMind(time_step);
    accum_time_ = 0.0f;
  }

  updateBody(time_step);

  accum_time_ += time_step * 0.001f;

}

// ------------------------------------------------------------------------- //


void Agent::draw(){

  GameObject::draw();

  if (NetworkGame::instance().game_menus_->debug_mode_) {
    switch (movement_mode_){
    case Agent::kMovementMode_Deterministic: {
      if (current_path_ != nullptr)
        current_path_->drawDebug();
      break;
    }
    
    case Agent::kMovementMode_Random: {
      if (current_path_ != nullptr)
        current_path_->drawDebug();

      current_box_.drawDebug(glm::vec4(0, 0, 255, 255));
      break;
    }
    
    case Agent::kMovementMode_Tracking: {
      //current_box_.drawDebug(Vector4(255, 0, 0, 255));

      break;
    }
    
    case Agent::kMovementMode_Pattern: {
      if (current_path_ != nullptr)
        current_path_->drawDebug();
      break;
    }
    case Agent::kMovementMode_NONE: {

      break;
    }
    default: {
      assert(false);
      break;
    }
    
    }
    
  }

}

// ------------------------------------------------------------------------- //

void Agent::updateMind(uint32_t time_step){
  // REMOVE: Prevent warnings until its used
  time_step;

  // FSM Application
  switch (fsm_state_){
  case Agent::kFSMState_Working: {
    FSM_Working();
    break;
  }
  case Agent::kFSMState_Chasing: {
    FSM_Chasing();
    break;
  }
  case Agent::kFSMState_Fleeing: {
    FSM_Fleeing();
    break;
  }
  case Agent::kFSMState_Resting: {
    FSM_Resting();
    break;
  }
  case Agent::kFSMState_End: {
    FSM_End();
    break;
  }
  case Agent::kFSMState_NONE: {
    assert(false);
    break;
  }
  default: {
    assert(false);
    break;
  }
  }
  
}

// ------------------------------------------------------------------------- //

void Agent::updateBody(uint32_t time_step){

  switch (movement_mode_) {
  case Agent::kMovementMode_Deterministic: {
    MOV_Deterministic(time_step);
    break;
  }

  case Agent::kMovementMode_Random: {
    MOV_Random(time_step);
    break;
  }

  case Agent::kMovementMode_Tracking: {
    MOV_Tracking(time_step);
    break;
  }

  case Agent::kMovementMode_Pattern: {
    MOV_Pattern(time_step);
    break;
  }

  case Agent::kMovementMode_NONE: {
    break;
  }
  default: {
    assert(false);
    break;
  }
  }

}

// ------------------------------------------------------------------------- //
/*
void Agent::editor(){

  Entity::editor();

  ImGui::Text("AGENT PARAMS");
  ImGui::Spacing();

  ImGui::Text("Target");
  ImGui::Indent(); ImGui::Text("X: %.2f, Y: %.2f", target_.x, target_.y); ImGui::Unindent();

  ImGui::Spacing();

  ImGui::PushID(2);
  ImGui::Text("Max acceleration (m/s)");
  ImGui::SliderFloat("", &max_acceleration_, 3.5f, 60.0f);
  ImGui::Indent(); ImGui::Text("Current acceleration: %2.3f m/s", acceleration_); ImGui::Unindent();
  ImGui::PopID();
  
  ImGui::Spacing();

  ImGui::Text("Brake Force");
  ImGui::PushID(3);
  ImGui::SliderFloat("", &brake_force_, 0.0f, 0.8f);
  ImGui::PopID();

  ImGui::Spacing();


  ImGui::Text("FSM PARAMS");
  ImGui::Spacing();

  switch (fsm_state_) {
  case Agent::kFSMState_Working: {
    ImGui::Indent(); ImGui::Text("Current state: WORKING"); ImGui::Unindent();
    switch (agent_size_) {
    case Agent::kAgentSize_Huge: {
      ImGui::Indent(); ImGui::Text("Movement mode: Deterministic"); ImGui::Unindent();
      break;
    }
    case Agent::kAgentSize_Normal: {
      ImGui::Indent(); ImGui::Text("Movement mode: Random"); ImGui::Unindent();
      break;
    }
    case Agent::kAgentSize_Small: {
      ImGui::Indent(); ImGui::Text("Movement mode: Pattern"); ImGui::Unindent();
      break;
    }
    case Agent::kAgentSize_NONE: {
      assert(false);
      break;
    }
    default: {
      assert(false);
      break;
    }
    }
    break;
  }
  case Agent::kFSMState_Chasing: {
    ImGui::Indent(); ImGui::Text("Current state: CHASING"); ImGui::Unindent();
    ImGui::Indent(); ImGui::Text("Chasing '%s'", objective_->name()); ImGui::Unindent();
    break;
  }
  case Agent::kFSMState_Fleeing: {
    ImGui::Indent(); ImGui::Text("Current state: FLEEING"); ImGui::Unindent();
    ImGui::Indent(); ImGui::Text("Fleeing from '%s'", objective_->name()); ImGui::Unindent();
    break;
  }
  case Agent::kFSMState_Resting: {
    ImGui::Indent(); ImGui::Text("Current state: RESTING"); ImGui::Unindent();
    ImGui::Indent(); ImGui::Text("Rest time: %f", (float)(rest_time_ / 1000)); ImGui::Unindent();
    ImGui::Indent(); ImGui::Text("Current rest time: %f", (float)(current_rest_time_ / 1000)); ImGui::Unindent();
    break;
  }
  case Agent::kFSMState_End: {

    break;
  }
  case Agent::kFSMState_NONE: {
    assert(false);
    break;
  }
  default: {
    assert(false);
    break;
  }
  }

  ImGui::Spacing();
  
  switch (agent_size_) {
  case Agent::kAgentSize_Huge: {
    ImGui::Indent(); ImGui::Text("Agent size: HUGE"); ImGui::Unindent();
    break;
  }
  case Agent::kAgentSize_Normal: {
    ImGui::Indent(); ImGui::Text("Agent size: NORMAL"); ImGui::Unindent();
    break;
  }
  case Agent::kAgentSize_Small: {
    ImGui::Indent(); ImGui::Text("Agent size: SMALL"); ImGui::Unindent();
    break;
  }
  case Agent::kAgentSize_NONE: {
    assert(false);
    break;
  }
  default: {
    assert(false);
    break;
  }
  }

}
*/
// ------------------------------------------------------------------------- //

void Agent::set_brake_force(float brake_force) {

  brake_force_ = glm::clamp(brake_force, 0.1f, 0.8f);
  
}

// ------------------------------------------------------------------------- //

void Agent::set_max_acceleration(float acceleration){

  max_acceleration_ = acceleration;

}

// ------------------------------------------------------------------------- //

void Agent::set_movement_mode(MovementMode movement_mode){

  movement_mode_ = movement_mode;

}

// ------------------------------------------------------------------------- //

void Agent::set_target(glm::vec2 target){

  target_ = target;

}

// ------------------------------------------------------------------------- //

void Agent::set_agent_size(AgentSize agent_size){

  agent_size_ = agent_size;

  switch (agent_size_) {
  case kAgentSize_Small: {
    transform_.scale_ = glm::vec3(0.75f, 0.75f, 0.75f);
    set_max_acceleration(kAgentDefaultSpeed * 1.25f);
    //Pattern movement
    set_movement_mode(Agent::kMovementMode_Pattern);
    set_brake_force(0.6f);
    set_mind_refresh_time(1.0f);
    break;
  }
  case kAgentSize_Normal: {
    transform_.scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
    set_max_acceleration(kAgentDefaultSpeed * 1.0f);
    //Random movement
    set_movement_mode(Agent::kMovementMode_Random);
    set_brake_force(0.5f);
    set_mind_refresh_time(1.0f);
    break;
  }
  case kAgentSize_Huge: {
    transform_.scale_ = glm::vec3(1.25f, 1.25f, 1.25f);
    set_max_acceleration(kAgentDefaultSpeed * 0.75f);
    //Deterministic movement
    set_movement_mode(Agent::kMovementMode_Deterministic);
    set_brake_force(0.4f);
    set_mind_refresh_time(1.0f);
    break;
  }
  case kAgentSize_NONE: {
    assert(false);
    break;
  }
  default: {
    assert(false);
    break;
  }
  }

}

// ------------------------------------------------------------------------- //

void Agent::set_deterministic_path(Path* absolute_path) {

  if (absolute_path != nullptr) current_path_ = absolute_path;

}

// ------------------------------------------------------------------------- //

void Agent::set_random_area(Box new_area){

  current_box_ = new_area;

}

// ------------------------------------------------------------------------- //

void Agent::set_tracking_objective(GameObject* objective){

  if (nullptr != objective) {
    objective_ = objective;
  }

}

// ------------------------------------------------------------------------- //

void Agent::set_tracking_area(Box new_area){

  current_box_ = new_area;

}

// ------------------------------------------------------------------------- //

void Agent::set_tracking(bool seek){

  seeking_ = seek;

}

// ------------------------------------------------------------------------- //

void Agent::addPattern(Pattern new_pattern){

  possible_patterns_.push_back(new_pattern);

}

// ------------------------------------------------------------------------- //

void Agent::set_mind_refresh_time(float time_in_seconds){

  refresh_time_ = time_in_seconds;
  accum_time_ = refresh_time_;

}

// ------------------------------------------------------------------------- //

glm::vec2 Agent::direction() const {
  
  return direction_;

}

// ------------------------------------------------------------------------- //

float Agent::acceleration() const {

  return acceleration_;

}

// ------------------------------------------------------------------------- //

 Agent::MovementMode Agent::movement_mode() const {

  return movement_mode_;

}

// ------------------------------------------------------------------------- //

glm::vec2 Agent::target() const {

  return target_;

}

// ------------------------------------------------------------------------- //

void Agent::MOV_Deterministic(uint32_t time_step) {

  if (current_path_ == nullptr) return;

  if (client_owner_id_ != NetworkGame::instance().client_id_) return;
  if (!NetworkGame::instance().game_started_ || !active_) return;

  // Init movement
  if (!started_movement_) {
    target_ = current_path_->next_target();
    started_movement_ = true;
  }

  glm::vec2 pos = glm::vec2(transform_.position_.x, transform_.position_.y);

	//Send command
  UnitData* unit_data = CreateUnitData(client_owner_id_, pos, unit_id_, 10, active_);
  NetworkGame::instance().unit_manager_->updateUnit(true, *unit_data);

  // Calculate movement
  if (!current_path_->is_ready()) {
    printf("\nError: agent deterministic path not set as ready");
    return;
  }
  

  if (glm::distance(pos, target_) < kDistanceToApproach) {
    // Reduce speed
    is_aproacching_target_ = true;
  }

  if (glm::distance(pos, target_) < kDistanceToReach) {
    is_aproacching_target_ = false;

    target_ = current_path_->next_target();
    
    // Check if has finished its path
    reached_target_ = current_path_->reached_target(target_);
    if (reached_target_) {
      reached_target_ = false;
      transform_.position_ = glm::vec3(current_path_->first_point(), 0);
      target_ = current_path_->at_point(1);
      active_ = false;
      // Send unit disactivate command
			UnitData* unit_data = CreateUnitData(client_owner_id_, pos, unit_id_, 0, false);
			NetworkGame::instance().unit_manager_->updateUnit(true, *unit_data);
      // Subtract to the active units count
      if (client_owner_id_ == 2) {
        NetworkGame::instance().unit_manager_->active_p2_units -= 1;
      }
      else {
        NetworkGame::instance().unit_manager_->active_p1_units -= 1;
      }
      // Check if units count is 0 then send the turn finished package
      NetworkGame::instance().unit_manager_->checkUnitsDisabled(true, client_owner_id_);
    }

  }

  desired_direction_ = target_ - pos;
  desired_direction_ = glm::normalize(desired_direction_);
  move(time_step);

}

// ------------------------------------------------------------------------- //

void Agent::MOV_Random(uint32_t time_step) {

  // Init movement
  if (!started_movement_) {
    current_box_.set_position(transform_.position_);
    current_box_.updatePoints();
    current_path_->clear();
    current_path_->addPoint(transform_.position_);
    glm::vec2 box_size = current_box_.size();
    for (uint32_t i = 0; i < 39; ++i) {
      // Rand with area
      glm::vec2 new_point = glm::vec2((float)(rand() % ((uint32_t)box_size.x)),
        (float)(rand() % ((uint32_t)box_size.y)));

      new_point -= glm::vec2(box_size.x / 2,
        box_size.y / 2);

      new_point += current_box_.position();
      current_path_->addPoint(new_point);
    }
    current_path_->set_direction(Path::kDirection_Forward);
    current_path_->set_action(Path::kAction_Straight);
    current_path_->set_ready();

    target_ = current_path_->first_point();
    started_movement_ = true;
  }


  // Calculate movement
  if (!current_path_->is_ready()) {
    printf("\nError: agent pattern not marked as ready");
    return;
  }

	glm::vec2 pos = glm::vec2(transform_.position_.x, transform_.position_.y);
  if (glm::distance(pos, target_) < kDistanceToApproach) {
    // Reduce speed
    is_aproacching_target_ = true;
  }

  if (glm::distance(pos, target_) < kDistanceToReach) {
    is_aproacching_target_ = false;

    // Check if has finished its path
    if (current_path_->reached_target(target_)) {
      started_movement_ = false;
      return;
    }

    target_ = current_path_->next_target();

  }

	desired_direction_ = target_ - pos;
	desired_direction_ = glm::normalize(desired_direction_);

  move(time_step);

}

// ------------------------------------------------------------------------- //

void Agent::MOV_Tracking(uint32_t time_step) {

  // Set chase objective and look for if its inside its box range
  // If is inside set as target and chase him
  if (objective_ != nullptr) {
    //if (current_box_.isPositionInside(objective_->position())) {
    target_ = objective_->transform_.position_;
    reached_target_ = false;
    //}
    //else {
    //  target_ = position_;
    //}
  }

  // Move vision area
  //current_box_.set_position(position_);
  //current_box_.updatePoints();

	glm::vec2 pos = glm::vec2(transform_.position_.x, transform_.position_.y);
  if (glm::distance(pos, target_) < kDistanceToReach) {
    //seeking_ = false;
    reached_target_ = true;
  }
  //if (glm::distance(pos, target_) > 300.0f && !seeking_) {
  //  seeking_ = true;
  //  reached_target_ = true;
  //}

	desired_direction_ = target_ - pos;
	desired_direction_ = glm::normalize(desired_direction_);

  if (!seeking_)
    desired_direction_ *= -1.0f;

  move(time_step);

}

// ------------------------------------------------------------------------- //

void Agent::MOV_Pattern(uint32_t time_step) {

  // Init movement
  if (!started_movement_) {
    if (current_pattern_ >= possible_patterns_.size())
      current_pattern_ = 0;

    current_path_->clear();
    if (possible_patterns_[current_pattern_].is_ready()) {
      current_path_->convertPatternToPath(transform_.position_,
        possible_patterns_[current_pattern_]);
    }

    current_path_->set_direction(Path::kDirection_Forward);
    current_path_->set_action(Path::kAction_Straight);
    current_path_->set_ready();

    target_ = current_path_->first_point();
    reached_target_ = false;
    is_aproacching_target_ = false;
    started_movement_ = true;
  }

  // Calculate movement
  if (!current_path_->is_ready()) {
    printf("\nError: agent pattern not marked as ready");
    return;
  }

	glm::vec2 pos = glm::vec2(transform_.position_.x, transform_.position_.y);
  if (glm::distance(pos, target_) < kDistanceToApproach) {
    // Reduce speed
    is_aproacching_target_ = true;
  }

  if (glm::distance(pos, target_) < kDistanceToReach) {
    is_aproacching_target_ = false;


    if (current_path_->reached_target(target_)) {
      current_pattern_++;
      reached_target_ = true;
      started_movement_ = false;
    }

    target_ = current_path_->next_target();

  }

	desired_direction_ = target_ - pos;
	desired_direction_ = glm::normalize(desired_direction_);

  move(time_step);

}

// ------------------------------------------------------------------------- //

void Agent::FSM_Working(){

  //Do only first time after transition
  if (is_transitioning_) {
    // Reset movements to default depending on its size
    switch (agent_size_) {
    case kAgentSize_Small: {
      set_movement_mode(Agent::kMovementMode_Pattern);
      break;
    }
    case kAgentSize_Normal: {
      set_movement_mode(Agent::kMovementMode_Random);
      break;
    }
    case kAgentSize_Huge: {
      set_movement_mode(Agent::kMovementMode_Deterministic);
      break;
    }
    case kAgentSize_NONE: {
      assert(false);
      break;
    }
    default: {
      assert(false);
      break;
    }
    }

    is_transitioning_ = false;
  }

  //Transitions
 /* if (BaseApp::Instance().current_scene()->quit_game_) {
    //Change to end
    fsm_state_ = kFSMState_End;
    is_transitioning_ = true;
  }

  std::vector<ref_ptr<Entity>> entities = BaseApp::Instance().current_scene()->entities_;
  //For each entity on scene
  for (uint32_t i = 0; i < entities.size(); ++i) {
    //Check if its an agent
    Agent* agent = dynamic_cast<Agent*>(entities[i].get());
    if (agent == nullptr) continue;

    //Check if its himself
    if (agent == this) continue;

    //Get distance to other agent
    float d = glm::vec2::Distance(position_, agent->position_);

    //Check if is bigger than other and d < d1
    if (agent_size_ > agent->agent_size_ && d < kDistanceOne) {
      //Change to chase
      fsm_state_ = kFSMState_Chasing;
      objective_ = agent;
      is_transitioning_ = true;
    }

    //Check if is smaller than other and d < d2
    if (agent_size_ < agent->agent_size_ && d < kDistanceTwo) {
      //Change to fleeing
      fsm_state_ = kFSMState_Fleeing;
      objective_ = agent;
      is_transitioning_ = true;
    }
  }*/

}

// ------------------------------------------------------------------------- //

void Agent::FSM_Chasing(){

  //Do only first time after transition
  if (is_transitioning_) {
    // Set tracking movement
    movement_mode_ = kMovementMode_Tracking;
    seeking_ = true;
    is_aproacching_target_ = false;

    is_transitioning_ = false;
  }

  //Transitions
  /*if (BaseApp::Instance().current_scene()->quit_game_) {
    //Change to end
    fsm_state_ = kFSMState_End;
    is_transitioning_ = true;
  }

  //Get distance to other agent
  float d = glm::vec2::Distance(position_, objective_->position());
  // Check for distance > d3
  if (d > kDistanceThree) {
    fsm_state_ = kFSMState_Resting;
    objective_ = nullptr;
    is_transitioning_ = true;
  }*/

}

// ------------------------------------------------------------------------- //

void Agent::FSM_Fleeing(){

  //Do only first time after transition
  if (is_transitioning_) {
    // Set inverse tracking movement (run away)
    movement_mode_ = kMovementMode_Tracking;
    seeking_ = false;
    is_aproacching_target_ = false;

    is_transitioning_ = false;
  }

  //Transitions
  /*if (BaseApp::Instance().current_scene()->quit_game_) {
    //Change to end
    fsm_state_ = kFSMState_End;
    is_transitioning_ = true;
  }

  //Get distance to other agent
  float d = glm::vec2::Distance(position_, objective_->position());
  // Check for distance > d3
  if (d > kDistanceThree) {
    fsm_state_ = kFSMState_Resting;
    objective_ = nullptr;
    is_transitioning_ = true;
  }*/

}

// ------------------------------------------------------------------------- //

void Agent::FSM_Resting(){

  //Do only first time after transition
  if (is_transitioning_) {
    // Set to movement mode none
    movement_mode_ = kMovementMode_NONE;
    //3 - 6 rest interval
    rest_time_ = 3000 + (rand() % 3000);
    current_rest_time_ = 0;
    is_transitioning_ = false;
  }
  /*
  //Update state
  current_rest_time_ += (uint32_t)refresh_time_ * 1000;

  //Transitions
  if (BaseApp::Instance().current_scene()->quit_game_) {
    //Change to end
    fsm_state_ = kFSMState_End;
    is_transitioning_ = true;
  }

  //Time
  if (current_rest_time_ > rest_time_) {
    fsm_state_ = kFSMState_Working;
    is_transitioning_ = true;
    started_movement_ = false;
  }

  //Only fleeing while resting
  std::vector<ref_ptr<Entity>> entities = BaseApp::Instance().current_scene()->entities_;
  //For each entity on scene
  for (uint32_t i = 0; i < entities.size(); ++i) {
    //Check if its an agent
    Agent* agent = dynamic_cast<Agent*>(entities[i].get());
    if (agent == nullptr) continue;

    //Check if its himself
    if (agent == this) continue;

    //Get distance to other agent
    float d = glm::vec2::Distance(position_, agent->position_);

    //Check if is smaller than other and d < d2
    if (agent_size_ < agent->agent_size_ && d < kDistanceTwo) {
      //Change to fleeing
      fsm_state_ = kFSMState_Fleeing;
      objective_ = agent;
      is_transitioning_ = true;
    }
  }*/

}

// ------------------------------------------------------------------------- //

void Agent::FSM_End(){

  //Do nothing, but FSM design requires an end state
  //if (is_transitioning_) {
  
  //}

}

// ------------------------------------------------------------------------- //

void Agent::move(uint32_t time_step){

  float delta_time = time_step * 0.001f;

  // Slow speed when approaching target
  if (is_aproacching_target_) {
    float decceleration = max_acceleration_ * (1.0f - brake_force_);
    acceleration_ += (0.95f * (decceleration - acceleration_)) * delta_time;
    //direction_.x += (0.99f * (desired_direction_.x - direction_.x)) * delta_time;
    //direction_.y += (0.99f * (desired_direction_.y - direction_.y)) * delta_time;
    direction_ = desired_direction_;
  }

  // Increase speed going to next target
  if (!reached_target_ && !is_aproacching_target_) {
    acceleration_ += (0.85f * (max_acceleration_ - acceleration_)) * delta_time;
    //direction_.x += (0.99f * (desired_direction_.x - direction_.x)) * delta_time;
    //direction_.y += (0.99f * (desired_direction_.y - direction_.y)) * delta_time;
    direction_ = desired_direction_;
  }

  // Brake when target is reached
  if (reached_target_) {
    acceleration_ += (0.95f * (0.0f - acceleration_)) * delta_time;
  }


  transform_.position_.x += acceleration_ * direction_.x * delta_time;
  transform_.position_.y += acceleration_ * direction_.y * delta_time;
  //transform_.rotation_ = glm::degrees(atan2(direction_.x, direction_.y));

}

// ------------------------------------------------------------------------- //
