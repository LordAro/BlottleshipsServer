/*
 * This file is part of BlottleshipsServer.
 * BAPS is licenced under MIT License. See LICENSE for more details.
 */

/** @file blottleships.cpp Functions related to starting BlottleshipsServer. */

#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace boost::asio;

class Manager;

/** Wrapper for a tcp socket connection. */
class Player : public std::enable_shared_from_this<Player>
{
public:
	/**
	 * Constructor.
	 * @param socket Socket to get data in/out of.
	 */
	Player(ip::tcp::socket socket, Manager &manager)
		: socket(std::move(socket)),
		  readbuf(1024),
		  writebuf(1024),
		  manager(manager)
	{}

	/** Start reading data from the socket. */
	void StartRead()
	{
		this->DoRead();
	}

	/**
	 * Setup data to be sent.
	 * @param s String to send. Must end with a newline.
	 */
	void Send(std::string s)
	{
		assert(s.back() == '\n');
		std::ostream req_write(&this->writebuf);
		req_write << s;
		this->DoWrite(s.size());
	}

private:
	/** Read data from the socket, and send it somewhere to be handled. */
	void DoRead()
	{
		auto self(shared_from_this());
		async_read_until(this->socket, this->readbuf, uint8_t('\n'),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec) {
					auto bufs = this->readbuf.data();
					std::string str(buffers_begin(bufs), buffers_begin(bufs) + length);
					boost::trim(str);
					std::cout << "Listened: " << str << std::endl;

					/* @todo Do stuff with str */
					this->readbuf.consume(length);
					this->Send("Test\n");

					this->DoRead(); // Continue reading (must be last)
				}
				/* There were errors, abort */
				std::cout << "Client disconnected!" << std::endl;
			});
	}

	/**
	 * Write data into the socket.
	 * @param length Length of data (in bytes) to write.
	 */
	void DoWrite(std::size_t length)
	{
		auto self(shared_from_this());
		async_write(this->socket, this->writebuf,
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
//				if (ec) do_something();
			});
	}

	ip::tcp::socket socket;     ///< Socket that we're connected with.
	basic_streambuf<> readbuf;  ///< Buffer for reading data.
	basic_streambuf<> writebuf; ///< Buffer for writing data that is to be sent.
	const Manager &manager;     ///< Reference to the Manager instance.
};

/** Wrapper around a tcp listener. */
class Manager
{
public:
	/**
	 * Constructor.
	 * @param io_service Socket to listen to new connections on.
	 * @param port Network port to listen on.
	 */
	Manager(io_service& io_service, short port)
		: acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port)),
		  socket(io_service)
	{
		this->DoAccept();
	}

private:
	/** Accept an incoming connection */
	void DoAccept()
	{
		this->acceptor.async_accept(this->socket,
			[this](boost::system::error_code ec)
			{
				if (!ec) {
					auto player = std::make_shared<Player>(std::move(this->socket), *this);
					player->StartRead();
					std::cout << "New client!" << std::endl;
				}
				this->DoAccept();
			});
	}

	ip::tcp::acceptor acceptor; ///< Listener.
	ip::tcp::socket socket;     ///< Socket to bind connections to.
};


/**
 * Entry point.
 * @param argc Number of commandline parameters.
 * @param argv Parameters.
 */
int main(int argc, char *argv[])
{
	std::cout << "Ohai" << std::endl;
	io_service io_serv;
	Manager s(io_serv, 4500);

	io_serv.run();
}
