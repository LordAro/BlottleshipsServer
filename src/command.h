/*
 * This file is part of BlottleshipsServer.
 * BlottleshipsServer is licenced under MIT License. See LICENSE for more details.
 */

/** @file player.cpp Definitons of a player. */

#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#include <boost/property_tree/json_parser.hpp>

class Command {
public:
	Command(const std::string &str);
private:
	boost::property_tree::ptree prop_tree; ///< Description of the command, from the JSON.
};
#endif /* COMMAND_H */
