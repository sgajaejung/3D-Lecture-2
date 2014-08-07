#pragma once


namespace network
{

	bool	LaunchClient(const std::string &ip, const int port, OUT SOCKET &out);

	bool LaunchServer(const int port, OUT SOCKET &out);

}
