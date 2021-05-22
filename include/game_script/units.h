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

// ------------------------------------------------------------------------- //

class UnitManager {
public:
	UnitManager();
	~UnitManager();

	void init();

	void drawPaths();

protected:
	void initUnits();
	void initPaths();

	Path* p1_path;
	Path* p2_path;

};

// ------------------------------------------------------------------------- //

#endif // __UNITS_H__