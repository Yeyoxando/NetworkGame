/*
 *	Author: Diego Ochando Torres
 *  Date: 16/03/2021
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include <WinSock2.h>

// ------------------------------------------------------------------------- //

#ifndef __NETWORK_DATA_H__
#define __NETWORK_DATA_H__

// ------------------------------------------------------------------------- //
/*
UnitData{ 
	id
	position, 
	life
	whatever needed to send 
}

BuildData{
	position
	build_kind
	hit_points
	etc
}

ServerUpdate{
	When the game ticks it'll send one of these
	It will make the players' buildings tick and consume/produce resources
}

*/

//Indicates the kind of the package
enum DataPackageKind {
	kDataPackageKind_Client = 0,
	kDataPackageKind_Transform,
	kDataPackageKind_Null
};

// ------------------------------------------------------------------------- //

//Client identifier
struct Client {
	int id;
};

// ------------------------------------------------------------------------- //

// Indicates the id of the player, and its position and direction
struct TransformTest {
	unsigned char id_player;
	short x;
	short y;
	unsigned char direction;
};

// ------------------------------------------------------------------------- //

// Package to send, can contain different information, identified later by the receiver
struct DataPackage {
	DataPackageKind package_kind;
	union { Client client; TransformTest transform; };
};

// ------------------------------------------------------------------------- //

#endif // __NETWORK_DATA_H__