/*
 * This file is part of BlottleshipsServer.
 * BlottleshipsServer is licenced under MIT License. See LICENSE for more details.
 */

/** @file manager.cpp Definitons of the game manager. */

#include <iostream>

#include "manager.h"
#include "player.h"

/**
 * Constructor.
 * @param io_service Socket to listen to new connections on.
 * @param port Network port to listen on.
 */
Manager::Manager(io_service& io_service, short port)
	: acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port)),
	  socket(io_service)
{
	this->DoAccept();
}

/**
 * Disconnects a player, and removes them from the list.
 * @param player Player to remove.
 */
void Manager::DisconnectPlayer(std::shared_ptr<Player> player)
{
	auto it = std::find(this->connections.begin(), this->connections.end(), player);
	assert(it != this->connections.end());
	this->connections.erase(it);
}

/** Accept an incoming connection */
void Manager::DoAccept()
{
	this->acceptor.async_accept(this->socket,
		[this](boost::system::error_code ec)
		{
			if (!ec) {
				auto player = std::make_shared<Player>(std::move(this->socket), *this);
				player->StartRead();
				this->connections.push_back(std::move(player));
				std::cout << "New client connected" << std::endl;
				this->DoAccept();
			}
		});
}

