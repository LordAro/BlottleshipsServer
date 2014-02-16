/*
 * This file is part of BlottleshipsServer.
 * BlottleshipsServer is licenced under MIT License. See LICENSE for more details.
 */

/** @file player.cpp Definitons of a player. */

#include <cassert>
#include <iostream>
#include <sstream>

#include "command.h"

/**
 * Construct a command from a string.
 * @param str The command, in raw string form.
 * @note May throw if command is invalid.
 */
Command::Command(const std::string &str)
{
	std::stringstream ss;
	ss << str;

	try {
		boost::property_tree::read_json(ss, this->prop_tree);
	} catch (boost::property_tree::json_parser::json_parser_error &je) {
		std::cout << "Error parsing command: " << je.message() << std::endl;
		throw;
	}
}
