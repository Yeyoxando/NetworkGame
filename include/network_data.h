/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <WinSock2.h>
#include <glm.hpp>
#include <vector>

// ------------------------------------------------------------------------- //

#ifndef __NETWORK_DATA_H__
#define __NETWORK_DATA_H__

// ------------------------------------------------------------------------- //
  // COMMANDS

//Client identifier
struct Client {
	int id;
};

struct CommandListHeader {
	int sender_id;
	int cmd_count_;
};

struct Command {
	int sender_id;
	int kind_;
};

struct StartGame : public Command {
	int start;
};

struct ServerTick : public Command { // Send when all player units are dead
	int tick;
};

struct BuildData : public Command {
	int x;
	int y;
	int build_kind;
};

struct UnitData : public Command {
	int x;
	int y;
	int id;
	int active;
	int hit_points;
};

// ------------------------------------------------------------------------- //

// Used to store the commands and execute them later when the net thread is not running
// When the net thread is running it will fill these lists instead, in parallel with the main thread
class CommandList {
public:
	CommandList() {
		commands_ = std::vector<Command*>(0);
	}

	~CommandList() { commands_.clear(); }

	std::vector<Command*> commands_;
};

// ------------------------------------------------------------------------- //

//Indicates the kind of the package sent through the network
enum DataPackageKind {
	kDataPackageKind_Client = 0,
	kDataPackageKind_StartGame,
	kDataPackageKind_ServerTick,
	kDataPackageKind_Header,
	kDataPackageKind_Build,
	kDataPackageKind_Unit,
	kDataPackageKind_Null
};

// Package to send, can contain different information, identified later by the receiver
struct DataPackage {
	DataPackageKind package_kind;

	union {
		Client client;
		ServerTick tick;
		StartGame start;
		CommandListHeader	header;
		BuildData build;
		UnitData unit;
	};

};

// ------------------------------------------------------------------------- //

// Commands creation to add them to the cmd list and send them when the net thread gets active.

static BuildData* CreateBuildData(int client_id, glm::vec2 position, int build_kind) {
	BuildData* data = new BuildData();
	data->kind_ = (int)DataPackageKind::kDataPackageKind_Build;
	data->sender_id = client_id;
	data->x = position.x;
	data->y = position.y;
	data->build_kind = build_kind;

	return data;
}

static UnitData* CreateUnitData(int client_id, glm::vec2 position, int unit_id, int life, bool active) {
	UnitData* data = new UnitData();
	data->kind_ = (int)DataPackageKind::kDataPackageKind_Unit;
	data->sender_id = client_id;
	data->x = position.x;
	data->y = position.y;
	data->id = unit_id;
	data->active = active;
	data->hit_points = life;

	return data;
}

// ------------------------------------------------------------------------- //

#endif // __NETWORK_DATA_H__