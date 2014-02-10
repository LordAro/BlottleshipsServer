/*
 * This file is part of BlottleshipsServer.
 * BAPS is licenced under MIT License. See LICENSE for more details.
 */

/** @file blottleships.cpp Functions related to starting BlottleshipsServer. */

#include <iostream>

#include <boost/asio.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace boost::asio;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(ip::tcp::socket socket)
		: socket(std::move(socket)),
		  readbuf(1024),
		  writebuf(1024)
	{}

	void StartRead()
	{
		this->DoRead();
	}

	void Send(std::string s)
	{
		assert(s.back() == '\n');
		std::ostream req_write(&this->writebuf);
		req_write << s;
		this->DoWrite(s.size());
	}

private:
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

					// TODO: Do stuff with str
					this->readbuf.consume(length);
					this->Send("Test\n");

					/* Continue reading (must be last) */
					this->DoRead();
				}
				/* There were errors, abort */
				std::cout << "Client disconnected!" << std::endl;
			});
	}

	void DoWrite(std::size_t length)
	{
		auto self(shared_from_this());
		async_write(this->socket, this->writebuf,
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
			{
//				if (ec) do_something();
			});
	}

	ip::tcp::socket socket;
	basic_streambuf<> readbuf;
	basic_streambuf<> writebuf;
};

class TcpServer
{
public:
	TcpServer(io_service& io_service, short port)
		: acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port)),
		  socket(io_service)
	{
		this->DoAccept();
	}

private:
	void DoAccept()
	{
		this->acceptor.async_accept(this->socket,
			[this](boost::system::error_code ec)
			{
				if (!ec) {
					std::make_shared<TcpConnection>(std::move(this->socket))->StartRead();
					std::cout << "New client!" << std::endl;
				}
				this->DoAccept();
			});
	}

	ip::tcp::acceptor acceptor;
	ip::tcp::socket socket;
};


int main(int argc, char *argv[])
{
	std::cout << "Ohai" << std::endl;
	io_service io_serv;
	TcpServer s(io_serv, 4500);

	io_serv.run();
}
