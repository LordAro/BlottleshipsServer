/*
 * This file is part of BlottleshipsServer.
 * BAPS is licenced under MIT License. See LICENSE for more details.
 */

/** @file blottleships.cpp Functions related to starting BlottleshipsServer. */

#include <iostream>

#include <boost/asio.hpp>

#include "manager.h"

/**
 * Entry point.
 * @param argc Number of commandline parameters.
 * @param argv Parameters.
 */
int main(int argc, char *argv[])
{
	std::cout << "Ohai" << std::endl;
	boost::asio::io_service io_serv;
	Manager s(io_serv, 4500);

	io_serv.run();
}
