/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __PATH_H__ 
#define __PATH_H__ 

#include <vector>
#include <glm.hpp>
#include <SDL.h>

class Pattern;

// ----------------------------------------------------------------------- //

/**
  * @brief: Path of 2D points to set Deterministic absolute paths or
  *            relative Pattern conducts.
  */
class Path{
 public:
  /// @brief: Path constructor
  Path();
  Path(Path& other);
  /// @brief: Path destructor
  virtual ~Path();

  /// @brief: max points that a path could have
  const uint16_t kMaxPoints = 200;

  /// @brief: determines on which direction path should be traveled
  enum Direction {
    kDirection_Forward = 0,
    kDirection_Backward = 1,
    kDirection_NONE = -1,
  };

  /// @brief: determines how many times path will be traveled
  enum Action {
    kAction_Straight = 0,
    kAction_LoopNTimes = 1,
    kAction_LoopInfinite = 2,
    kAction_NONE = -1,
  };
    
  /// @brief: delete all current path info
  void clear();

  /// @brief: converts a pattern into a path
  void convertPatternToPath(glm::vec2 agent_position, Pattern pattern);

  /// @brief: draws the path
  void drawDebug();

  /// @brief: editor for the path
  void editor();

  /**
    * @brief: Add a new point to the path
    * @param: point to add
    */
  void addPoint(glm::vec2 point);

  /**
    * @brief: calculates the transformed points respect to the camera and world
    * @param: view matrix from the camera
    */
  void calculateTransformedPoints(/*glm::mat3 view_matrix*/);

  // ------------------------------ Setters ------------------------------ //

  /// @brief: allows the path to start 
  void set_ready();
  /// @brief: Sets Path direction
  void set_direction(Direction direction);
  /// @brief: Sets Path action
  void set_action(Action action);
  /// @brief: Sets Path Action with a number of repetitions
  void set_action(Action action, uint32_t loops);

  // ------------------------------ Getters ------------------------------ //

  /**
    * @brief: Query for the next target
    * @return: the next target for the current path
    */
  glm::vec2 next_target();
  /**
    * @brief: Check if the path is finished
    * @return: true if the path is finished, depends on Path Direction and Action
    */
  bool reached_target(glm::vec2 target);

  /// @result: number of points of Path
  uint32_t number_points() const;
  /// @result: first point of Path
  glm::vec2 first_point() const;
  /// @result: Point at position of Path
  glm::vec2 at_point(uint32_t index) const;
  /// @result: last point of Path
  glm::vec2 last_point() const;

  /// @result: next point traveling Path
  glm::vec2 next_point();
  /// @result: previous point traveling Path
  glm::vec2 prev_point();

  /// @result: true if Path marked as ready
  bool is_ready() const;
  /// @result: true if given point is equal to Path first point
  bool is_first(glm::vec2 point) const;
  /// @result: true if given point is equal to Path last point
  bool is_last(glm::vec2 point) const;

 private:
  /// @brief: Direction of Path
  Direction direction_;
  /// @brief: Action of Path
  Action action_;
  /// @brief: Current index of traveling Path
  int32_t current_index_;
  /// @brief: Number of repetitions Path should make
  uint32_t action_loops_;
  /// @brief: Current number of repetitions performed
  uint32_t current_loops_;
  /// @brief: Number of points of Path
  uint32_t number_points_;
  /// @brief: World points of Path
  std::vector<glm::vec2> points_;
  /// @brief: Is ready to start
  bool is_ready_;

  // --- Debug ---
  /// @brief: Stores view transformed points calculated at Step to use it later on Draw
  std::vector<SDL_Point> transformed_points_;

};

// ----------------------------------------------------------------------- //

#endif //__PATH_H__
