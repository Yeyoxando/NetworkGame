//Indicates the kind of the package
enum DataPackageKind {
	kDataPackageKind_Client = 0,
	kDataPackageKind_Transform,
	kDataPackageKind_Null
};

//Client identifier
struct Client {
	int id;
};

// Indicates the id of the player, and its position and direction
struct Transform {
	unsigned char id_player;
	short x;
	short y;
	unsigned char direction;
};

// Package to send, can contain different information, identified later by the receiver
struct DataPackage {
	DataPackageKind package_kind;
	union { Client client; Transform transform; };
};

// ------------------------------------------------------------------------- //

//Returns a new ip settled with the given parameters
sockaddr_in SetIP(const char* ip, int port) {
	struct sockaddr_in new_ip;

	new_ip.sin_family = AF_INET;
	new_ip.sin_addr.s_addr = inet_addr(ip);
	new_ip.sin_port = htons(port);

	return new_ip;
}

// ------------------------------------------------------------------------- //
