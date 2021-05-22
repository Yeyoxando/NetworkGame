/*
 *	Author: Diego Ochando Torres
 *  Date: 09/05/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */


// ------------------------------------------------------------------------- //

#include "units.h"

// ------------------------------------------------------------------------- //

UnitManager::UnitManager(){

	p1_path = nullptr;
	p2_path = nullptr;

}

// ------------------------------------------------------------------------- //

UnitManager::~UnitManager(){

	delete p1_path;
	delete p2_path;

}

// ------------------------------------------------------------------------- //

void UnitManager::init(){

	initPaths();
	initUnits();

}

// ------------------------------------------------------------------------- //

void UnitManager::drawPaths(){

	p1_path->drawDebug();
	p2_path->drawDebug();

}

// ------------------------------------------------------------------------- //

void UnitManager::initUnits(){



}

// ------------------------------------------------------------------------- //

void UnitManager::initPaths(){

	p1_path = new Path();
	p1_path->addPoint(glm::vec2(88, 376));
	p1_path->addPoint(glm::vec2(88, 360));
	p1_path->addPoint(glm::vec2(104, 360));
	p1_path->addPoint(glm::vec2(104, 344));
	p1_path->addPoint(glm::vec2(120, 344));
	p1_path->addPoint(glm::vec2(136, 344));
	p1_path->addPoint(glm::vec2(152, 344));
	p1_path->addPoint(glm::vec2(168, 344));
	p1_path->addPoint(glm::vec2(184, 344));
	p1_path->addPoint(glm::vec2(184, 360));
	p1_path->addPoint(glm::vec2(200, 360));
	p1_path->addPoint(glm::vec2(216, 360));
	p1_path->addPoint(glm::vec2(232, 360));
	p1_path->addPoint(glm::vec2(248, 360));
	p1_path->addPoint(glm::vec2(264, 360));
	p1_path->addPoint(glm::vec2(280, 360));
	p1_path->addPoint(glm::vec2(296, 360));
	p1_path->addPoint(glm::vec2(312, 360));
	p1_path->addPoint(glm::vec2(328, 360));
	p1_path->addPoint(glm::vec2(344, 360));
	p1_path->addPoint(glm::vec2(360, 360));
	p1_path->addPoint(glm::vec2(376, 360));
	p1_path->addPoint(glm::vec2(392, 360));
	p1_path->addPoint(glm::vec2(408, 360));
	p1_path->addPoint(glm::vec2(424, 360));
	p1_path->addPoint(glm::vec2(440, 360));
	p1_path->addPoint(glm::vec2(456, 360));
	p1_path->addPoint(glm::vec2(472, 360));
	p1_path->addPoint(glm::vec2(488, 360));
	p1_path->addPoint(glm::vec2(504, 360));
	p1_path->addPoint(glm::vec2(504, 344));
	p1_path->addPoint(glm::vec2(504, 328));
	p1_path->addPoint(glm::vec2(504, 312));
	p1_path->addPoint(glm::vec2(488, 312));
	p1_path->addPoint(glm::vec2(488, 296));
	p1_path->addPoint(glm::vec2(472, 296));
	p1_path->addPoint(glm::vec2(456, 296));
	p1_path->addPoint(glm::vec2(456, 280));
	p1_path->addPoint(glm::vec2(440, 280));
	p1_path->addPoint(glm::vec2(440, 264));
	p1_path->addPoint(glm::vec2(440, 248));
	p1_path->addPoint(glm::vec2(440, 232));
	p1_path->addPoint(glm::vec2(456, 232));
	p1_path->addPoint(glm::vec2(456, 216));
	p1_path->addPoint(glm::vec2(472, 216));
	p1_path->addPoint(glm::vec2(488, 216));
	p1_path->addPoint(glm::vec2(504, 216));
	p1_path->addPoint(glm::vec2(504, 200));
	p1_path->addPoint(glm::vec2(520, 200));
	p1_path->addPoint(glm::vec2(520, 184));
	p1_path->addPoint(glm::vec2(536, 184));
	p1_path->addPoint(glm::vec2(536, 168));
	p1_path->addPoint(glm::vec2(536, 152));
	p1_path->addPoint(glm::vec2(536, 136));
	p1_path->addPoint(glm::vec2(536, 120));
	p1_path->addPoint(glm::vec2(552, 120));
	p1_path->addPoint(glm::vec2(552, 104));
	p1_path->addPoint(glm::vec2(552, 88));
	p1_path->addPoint(glm::vec2(552, 72));
	p1_path->addPoint(glm::vec2(552, 56));
	p1_path->is_ready();
	p1_path->calculateTransformedPoints();

	p2_path = new Path();
	p2_path->addPoint(glm::vec2(520, 24));
	p2_path->addPoint(glm::vec2(520, 40));
	p2_path->addPoint(glm::vec2(504, 40));
	p2_path->addPoint(glm::vec2(504, 56));
	p2_path->addPoint(glm::vec2(488, 56));
	p2_path->addPoint(glm::vec2(472, 56));
	p2_path->addPoint(glm::vec2(472, 40));
	p2_path->addPoint(glm::vec2(456, 40));
	p2_path->addPoint(glm::vec2(440, 40));
	p2_path->addPoint(glm::vec2(440, 56));
	p2_path->addPoint(glm::vec2(440, 56));
	p2_path->addPoint(glm::vec2(424, 56));
	p2_path->addPoint(glm::vec2(408, 56));
	p2_path->addPoint(glm::vec2(392, 56));
	p2_path->addPoint(glm::vec2(376, 56));
	p2_path->addPoint(glm::vec2(376, 72));
	p2_path->addPoint(glm::vec2(360, 72));
	p2_path->addPoint(glm::vec2(344, 72));
	p2_path->addPoint(glm::vec2(328, 72));
	p2_path->addPoint(glm::vec2(312, 72));
	p2_path->addPoint(glm::vec2(296, 72));
	p2_path->addPoint(glm::vec2(280, 72));
	p2_path->addPoint(glm::vec2(264, 72));
	p2_path->addPoint(glm::vec2(248, 72));
	p2_path->addPoint(glm::vec2(232, 72));
	p2_path->addPoint(glm::vec2(216, 72));
	p2_path->addPoint(glm::vec2(200, 72));
	p2_path->addPoint(glm::vec2(184, 72));
	p2_path->addPoint(glm::vec2(168, 72));
	p2_path->addPoint(glm::vec2(152, 72));
	p2_path->addPoint(glm::vec2(136, 72));
	p2_path->addPoint(glm::vec2(120, 72));
	p2_path->addPoint(glm::vec2(120, 88));
	p2_path->addPoint(glm::vec2(104, 88));
	p2_path->addPoint(glm::vec2(104, 104));
	p2_path->addPoint(glm::vec2(88, 104));
	p2_path->addPoint(glm::vec2(88, 120));
	p2_path->addPoint(glm::vec2(88, 136));
	p2_path->addPoint(glm::vec2(88, 152));
	p2_path->addPoint(glm::vec2(104, 152));
	p2_path->addPoint(glm::vec2(104, 168));
	p2_path->addPoint(glm::vec2(120, 168));
	p2_path->addPoint(glm::vec2(136, 168));
	p2_path->addPoint(glm::vec2(136, 184));
	p2_path->addPoint(glm::vec2(152, 184));
	p2_path->addPoint(glm::vec2(152, 200));
	p2_path->addPoint(glm::vec2(152, 216));
	p2_path->addPoint(glm::vec2(152, 232));
	p2_path->addPoint(glm::vec2(152, 248));
	p2_path->addPoint(glm::vec2(136, 248));
	p2_path->addPoint(glm::vec2(136, 264));
	p2_path->addPoint(glm::vec2(120, 264));
	p2_path->addPoint(glm::vec2(120, 280));
	p2_path->addPoint(glm::vec2(104, 280));
	p2_path->addPoint(glm::vec2(88, 280));
	p2_path->addPoint(glm::vec2(72, 280));
	p2_path->addPoint(glm::vec2(72, 296));
	p2_path->addPoint(glm::vec2(56, 296));
	p2_path->addPoint(glm::vec2(56, 312));
	p2_path->addPoint(glm::vec2(56, 328));
	p2_path->addPoint(glm::vec2(56, 344));
	p2_path->is_ready();
	p2_path->calculateTransformedPoints();

}

// ------------------------------------------------------------------------- //

