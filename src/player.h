/*
 * This file is part of BlottleshipsServer.
 * BlottleshipsServer is licenced under MIT License. See LICENSE for more details.
 */

/** @file blottleships.cpp Declaration of functions relating to Players. */

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>

#include <boost/asio.hpp>

using namespace boost::asio;

class Manager;

/** Wrapper for a tcp socket connection. */
class Player : public std::enable_shared_from_this<Player>
{
public:
	Player(ip::tcp::socket socket, Manager &manager);

	void StartRead();
	void Send(std::string s);

private:
	void DoRead();
	void DoWrite(std::size_t length);

	ip::tcp::socket socket;     ///< Socket that we're connected with.
	basic_streambuf<> readbuf;  ///< Buffer for reading data.
	basic_streambuf<> writebuf; ///< Buffer for writing data that is to be sent.
	Manager &manager;           ///< Reference to the Manager instance.
};

#endif /* PLAYER_H */
