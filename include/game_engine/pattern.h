/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __PATTERN_H__ 
#define __PATTERN_H__

// ------------------------------------------------------------------------- //

#include <vector>

// ----------------------------------------------------------------------- //
 
/// @brief: Class for patterns management.
class Pattern {
 public:
  /// @brief: Pattern constructor
  Pattern();
  /// @brief: Pattern destructor
  ~Pattern();

  /// @brief: possible directions for commands.
  enum PatternDirection {
    kPatternDirection_North = 0,
    kPatternDirection_NorthEast = 1,
    kPatternDirection_East = 2,
    kPatternDirection_SouthEast = 3,
    kPatternDirection_South = 4,
    kPatternDirection_SouthWest = 5,
    kPatternDirection_West = 6,
    kPatternDirection_NorthWest = 7,
  };
    
  /// @brief: Command to create patterns.
  struct PatternCommand {
    /// @brief: PatternCommand constructor
    PatternCommand();
    /// @brief: PatternCommand destructor
    PatternCommand(PatternDirection dir, float dist);

    /// @brief: Command direction
    PatternDirection direction_;
    /// @brief: Command distance to travel
    float distance;
  };

  /// @brief: add a command to current pattern
  void addCommand(PatternCommand command);

  /// @brief: delete all current pattern info
  void clear();

  /// @brief: mark the path ready to use
  void set_ready();

  /// @result: true if pattern can start 
  bool is_ready() const;

  /// @brief: Current commands that form Pattern
  std::vector<PatternCommand> commands_;
  
 private:
  /// @brief: if true pattern can start
  bool is_ready_;

};

// ----------------------------------------------------------------------- //

#endif //__PATTERN_H__
