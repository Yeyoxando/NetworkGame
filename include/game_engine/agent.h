/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __AGENT_H__
#define __AGENT_H__ 

#include "box.h"
#include "path.h"
#include "pattern.h"
#include "game_object.h"

// ----------------------------------------------------------------------- //

/// @brief: Autonomous agent AI implementation with different movement behaviors.
class Agent : public GameObject {
  friend class Scene;

 public:
  /// @brief: Agent constructor
  Agent();

  /// @brief: Agent destructor
  virtual ~Agent();

  // --- General movement constants ---
  /// @brief: Generic speed for an agent 
  const float kAgentDefaultSpeed = 20.0f;
  /// @brief: Distance to check if target is reached 
  const float kDistanceToReach = 5.0f;
  /// @brief: Distance to check if target is near and should slow their speed
  const float kDistanceToApproach = 10.0f;

  // --- FSM movement constants
  /// @brief: Distance to start chasing for bigger agents
  const float kDistanceOne = 200.0f;
  /// @brief: Distance to start fleeing for smaller agents
  const float kDistanceTwo = 150.0f;
  /// @brief: Distance to stop "viewing" the objective
  const float kDistanceThree = 220.0f;

  /// @brief: Determine agent movement behavior.
  enum MovementMode {
    kMovementMode_Deterministic = 0,
    kMovementMode_Random = 1,
    kMovementMode_Tracking = 2,
    kMovementMode_Pattern = 3,
    kMovementMode_NONE = -1,
  };

  /// @brief: Determine agent state for FSM.
  enum FSMState {
    kFSMState_Working = 0,
    kFSMState_Chasing = 1,
    kFSMState_Fleeing = 2,
    kFSMState_Resting = 3,
    kFSMState_End = 4,
    kFSMState_NONE = -1,
  };

  /// @brief: Determines agent size.
  enum AgentSize {
    kAgentSize_Small = 0,
    kAgentSize_Normal = 1,
    kAgentSize_Huge = 2,
    kAgentSize_NONE = -1,
  };

  /**
    * @brief: Calculates agent actions. Executed in steps with given refresh time.
    * @param: time_step to allow physic movement calculations
    */
  void updateMind(uint32_t time_step);

  /**
    * @brief: Executes and apply agent actions. Executed all frames.
    * @param: time_step to allow physic movement calculations
    */
  void updateBody(uint32_t time_step);

  /// @brief: Displays and modifies all the agents settings.
  //virtual void editor() override;

  // ------------------------------ Setters ------------------------------ //

  // --- Common setters ---
  /// @brief: Set agent brake force
  void set_brake_force(float brake_force);
  /// @brief: Set agent max acceleration
  void set_max_acceleration(float acceleration);

  /// @brief: Set agent movement mode
  void set_movement_mode(MovementMode movement_mode);
  /// @brief: Set agent target
  void set_target(glm::vec2 target);

  /// @brief: Set agent size
  void set_agent_size(AgentSize agent_size);

  /// @brief: Set Agent mind refresh time
  void set_mind_refresh_time(float time_in_seconds);

  // --- Setters for Deterministic movement ---
  /// @brief: Set Deterministic movement path
  void set_deterministic_path(Path* absolute_path);

  // --- Setters for Random Movement ---
  /// @brief: Set random movement area
  void set_random_area(Box new_area);

  // --- Setters for Tracking Movement ---
  /// @brief: Set Tracking objective
  void set_tracking_objective(GameObject* objective);
  /// @brief: Set agent field of view area
  void set_tracking_area(Box new_area);
  /// @brief: Set agent seek (true) or hide (false)
  void set_tracking(bool seek); 

  // --- Setters for Pattern Movement ---
  /// @brief: add new pattern possible movement
  void addPattern(Pattern new_pattern);


  // ------------------------------ Getters ------------------------------ //

  // --- Common getters ---
  /// @result: Agent direction
  glm::vec2 direction() const;
  /// @result: Agent current acceleration
  float acceleration() const;

  /// @result: Agent movement mode
  MovementMode movement_mode() const;
  /// @result: Agent current target
  glm::vec2 target() const;

 protected:
  /**
    * @brief: override Entity step to execute agent logic
    * @param: time_step for logic update calculations
    */
  virtual void update(uint32_t time_step) override;

  /**
    * @brief: override Entity Draw to draw debug info too
    * @param: if true draws some debug info like paths or boxes
    */
  virtual void draw() override;

  // ------------------------------ Movement ----------------------------- //

  /**
    * @brief: Deterministic conduct: predefined behavior with predictable results.
    *         Uses a given path with to perform its movement.
    * @param: time_step for logic update calculations
    */
  void MOV_Deterministic(uint32_t time_step);
    
  /**
    * @brief: Random conduct: unpredictable movement inside an scoped area, calculates a path
    *         of random positions, when it reach the end it generates anther one.
    * @param: time_step for logic update calculations
    */
  void MOV_Random(uint32_t time_step);
    
  /**
    * @brief: Tracking conduct: detect if the given objective is inside their vision area and
    *         follow it. When the objective go out of the area it will stop.
    * @param: time_step for logic update calculations
    */
  void MOV_Tracking(uint32_t time_step);
    
  /**
    * @brief: Pattern conduct: Follow a sequence of predefined steps.
    *         It could have different patterns stored an execute one after another.
    * @param: time_step for logic update calculations
    */
  void MOV_Pattern(uint32_t time_step);


  /// @brief: Working state for FSM (Default state)
  void FSM_Working();
  /// @brief: Chasing state for FSM 
  void FSM_Chasing();
  /// @brief: Fleeing state for FSM 
  void FSM_Fleeing();
  /// @brief: Resting state for FSM 
  void FSM_Resting();
  /// @brief: End state for FSM 
  void FSM_End();


  /**
    * @brief: moves the agent
    * @param: time_step for logic update calculations
    */
  void move(uint32_t time_step);

  // ---------------------------- Attributes ----------------------------- //

  // --- Mind management ---
  /// @brief: Time at Mind should update
  float refresh_time_;
  /// @brief: current time since last mind Update
  float accum_time_;

  // --- Common movement ---
  /// @brief: current agent direction
  glm::vec2 direction_;
  /// @brief: Desired direction that current has to reach
  glm::vec2 desired_direction_;
  /// @brief: Current agent acceleration (Speed)
  float acceleration_;
  /// @brief: Max speed that agent could reach
  float max_acceleration_;
  /// @brief: Force to brake
  float brake_force_;

  /// @brief: Agent target
  glm::vec2 target_;
  /// @brief: true if agent has reached its final target
  bool reached_target_;
  /// @brief: true if is near to target
  bool is_aproacching_target_;

  /// @brief: true if movement settings are configured
  bool started_movement_;
  /// @brief: Agent movement mode
  MovementMode movement_mode_;

  /// @brief: Agent size
  AgentSize agent_size_;

  /// @brief: Agent current Path
  Path* current_path_;
  /// @brief: Agent current Box
  Box current_box_;

  // --- Tracking movement ---
  /// @brief: Tracking movement objective
  
  GameObject* objective_;
  /// @brief: True if is seeking, false if is running away
  bool seeking_;

  // --- Pattern movement ---
  /// @brief: Current Pattern for pattern movement
  uint32_t current_pattern_;
  /// @brief: Pool of possible patterns
  std::vector<Pattern> possible_patterns_;



  // --- FSM ---
  /// @brief: FSM current state
  FSMState fsm_state_;
  /// @brief: check for only do transitions settings once
  bool is_transitioning_;

  /// @brief: time to rest on resting state (milliseconds)
  uint32_t rest_time_;
  /// @brief: time that agent has rest currently
  uint32_t current_rest_time_;

};

// ----------------------------------------------------------------------- //

#endif // __AGENT_H__
