/*
 *	Author: Diego Ochando Torres
 *  Date: 09/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __UNITS_H__
#define __UNITS_H__

// ------------------------------------------------------------------------- //

#include "path.h"
#include "agent.h"
#include "network_data.h"

#include <vector>

// ------------------------------------------------------------------------- //

class UnitManager {
public:
	UnitManager();
	~UnitManager();

	const int kUnitFoodCost = 2;
	const float kUnitSpawnTime = 3.0f;

	void init(int client_id);

	void update(uint32_t time_step);

	void drawPaths();

	Agent* createUnit(bool send_command, int client_id);

	void updateUnit(bool send_command, UnitData& data);

	void killUnit(bool send_command, UnitData& data);

	bool isUnitCreated(int player_id, int unit_id);

	void checkUnitsDisabled(bool send_command, int client_id);

	void reactivateUnits(int client_id, bool first_unit);

protected:
	void initUnits();
	void initPaths();

	int active_p1_units;
	int active_p2_units;

	Path* p1_path_;
	Path* p2_path_;

	std::vector<Agent*> agents_p1_;
	std::vector<Agent*> agents_p2_;

	int client_id_;

	float accum_time_;
	bool keep_spawning_;

	friend class NetworkGame;
	friend class Agent;

};

// ------------------------------------------------------------------------- //

#endif // __UNITS_H__