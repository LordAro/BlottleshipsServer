/*
 * This file is part of BlottleshipsServer.
 * BAPS is licenced under MIT License. See LICENSE for more details.
 */

/** @file manager.h Function declarations of the game manager. */

#ifndef MANAGER_H
#define MANAGER_H

#include <list>
#include <memory>

#include <boost/asio.hpp>

#include "player.h"

using namespace boost::asio;

/** Wrapper around a tcp listener. */
class Manager
{
public:
       Manager(io_service& io_service, short port);

private:
       void DoAccept();

       ip::tcp::acceptor acceptor; ///< Listener.
       ip::tcp::socket socket;     ///< Socket to bind connections to.

       std::list<std::shared_ptr<Player>> connections; ///< List of alive Player connections.
};

#endif /* MANAGER_H */
