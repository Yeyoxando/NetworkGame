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

struct BuildData : public Command {
	int x;
	int y;
	//build_kind
	//hit_points
	//etc
};

/*
UnitData : public Command{ 
	id
	position, 
	life
	whatever needed to send 
}

ServerUpdate{
	When the game ticks it'll send one of these
	It will make the players' buildings tick and consume/produce resources
}

*/

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
	kDataPackageKind_Header,
	kDataPackageKind_Build,
	kDataPackageKind_Null
};

// Package to send, can contain different information, identified later by the receiver
struct DataPackage {
	DataPackageKind package_kind;

	union {
		Client client;
		CommandListHeader	header;
		BuildData build;
	};

};

// ------------------------------------------------------------------------- //

// Commands creation to add them to the cmd list and send them when the net thread gets active.

static BuildData* CreateBuildData(int client_id, glm::vec2 position) {
	BuildData* data = new BuildData();
	data->kind_ = (int)DataPackageKind::kDataPackageKind_Build;
	data->sender_id = client_id;
	data->x = position.x;
	data->y = position.y;
	return data;
}

// ------------------------------------------------------------------------- //

#endif // __NETWORK_DATA_H__