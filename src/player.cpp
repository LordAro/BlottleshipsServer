/*
 * This file is part of BlottleshipsServer.
 * BlottleshipsServer is licenced under MIT License. See LICENSE for more details.
 */

/** @file player.cpp Definitons of a player. */

#include <boost/algorithm/string/trim.hpp>

#include "command.h"
#include "player.h"
#include "manager.h"

/**
 * Constructor.
 * @param socket Socket to get data in/out of.
 */
Player::Player(ip::tcp::socket socket, Manager &manager)
	: socket(std::move(socket)),
	  readbuf(1024),
	  writebuf(1024),
	  manager(manager)
{}

/** Start reading data from the socket. */
void Player::StartRead()
{
	this->DoRead();
}

/**
 * Setup data to be sent.
 * @param s String to send. Must end with a newline.
 */
void Player::Send(std::string s)
{
	assert(s.back() == '\n');
	std::ostream req_write(&this->writebuf);
	req_write << s;
	this->DoWrite(s.size());
}

/** Read data from the socket, and send it somewhere to be handled. */
void Player::DoRead()
{
	auto self(shared_from_this());
	async_read_until(this->socket, this->readbuf, uint8_t('\n'),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec) {
				auto bufs = this->readbuf.data();
				std::string str(buffers_begin(bufs), buffers_begin(bufs) + length);
				this->readbuf.consume(length);
				boost::trim(str);
				std::cout << "Received: " << str << std::endl;

				std::unique_ptr<Command> cmd = nullptr;
				try {
					cmd = std::unique_ptr<Command>(new Command(str));
				} catch (boost::property_tree::json_parser::json_parser_error &je) {
					/* @todo Deal with invalid command. */
				}

				/* @todo Do stuff with str */
				this->Send("echo: " + str + '\n');

				this->DoRead(); // Continue reading (must be last)
			} else {
				/* There were errors, abort */
				this->manager.DisconnectPlayer(self);
				std::cout << "Client disconnected: " << ec.message() << std::endl;
			}
		});
}

/**
 * Write data into the socket.
 * @param length Length of data (in bytes) to write.
 */
void Player::DoWrite(std::size_t length)
{
	auto self(shared_from_this());
	async_write(this->socket, this->writebuf,
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
//				if (ec) do_something();
		});
}
