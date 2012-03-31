#ifndef SERIAL_PORT_DOT_H
#define SERIAL_PORT_DOT_H
#define _WIN32_WINNT 0x0501
#include <SDL.h>
#include <boost/circular_buffer.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "lua_wrapper.h"

const int OUT_BUFFER_SIZE = 256;
const int IN_BUFFER_SIZE = 256;

class serial_port
{
	protected:
	
		//boost asio shit
		static boost::thread th;
		static boost::asio::io_service io;
		static void threadfunc();
		static boost::asio::io_service::work wrk;

		//port shit
		boost::asio::serial_port port;
		char inbuff[IN_BUFFER_SIZE];
		boost::circular_buffer<char> outbuffer;
		
	public:
		static void stop();		
		serial_port(const char* filename);
		void write(const char* buffer, int len);
		
		void onwrite(const boost::system::error_code& error,std::size_t bytes_transferred);
		void onread(const boost::system::error_code& error,std::size_t bytes_transferred);
		
		//lua shit
		const static char* METATABLE_NAME;
		
		static void register_serialport(lua_wrapper& L);
		static int new_serialport(lua_State *L);
		static int collect_serialport(lua_State *L);
		static int read_event(lua_State *L);
		static int lua_write(lua_State *L);
};

#endif