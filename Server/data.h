#pragma once

// Common data structures

namespace BTService {

	/*!
	* The BT Service Request datagram
	*/
	struct Request
	{
		int client_id; // Client id
		int key; // Key number
	};

	/*!
	* The BT Service Response datagram
	*/
	struct Response
	{
		int avg; // the average of squares
	};

	constexpr int KEY_FROM = 0;
	constexpr int KEY_TO = 1023;
	constexpr char* PERSISTENCE_FILENAME = "bt.bin";

}