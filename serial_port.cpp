#include "serial_port.h"
#include <iostream>

serial_read_event::serial_read_event(serial_port* emitter):
event(),
home_port(emitter)
{

}

void serial_read_event::handle(int iobytes)
{
	home_port->on_read();
}

serial_write_event::serial_write_event(serial_port* emitter):
event(),
home_port(emitter)
{

}

void serial_write_event::handle(int iobytes)
{
	home_port->on_write();
}

serial_port::serial_port(event_loop* looper,const char* portname):
read_ev(this),
write_ev(this)
{
	port_handle = CreateFileA(portname, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

	if(port_handle == INVALID_HANDLE_VALUE)
	{
		throw std::exception("port open failed");
	}

	HANDLE iocp_success = CreateIoCompletionPort(port_handle,looper->get_handle(),event_loop::EVENT_KEY,1);

	if(iocp_success != looper->get_handle())
	{
		//close the port handle before throwing
		CloseHandle(port_handle);

		throw std::exception("add port to iocp failed");
	}

	//ok set the timeouts
	COMMTIMEOUTS timeouts;
	memset((void*)&timeouts,0x00,sizeof(COMMTIMEOUTS));

	timeouts.ReadIntervalTimeout = 5;
	timeouts.ReadTotalTimeoutConstant = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
	timeouts.WriteTotalTimeoutConstant = MAXDWORD;
	timeouts.WriteTotalTimeoutMultiplier = MAXDWORD;

	SetCommTimeouts(port_handle,&timeouts);

	start_read();
}

void serial_port::start_read()
{
	memset(in_buff,0,sizeof(in_buff));
	ReadFile(port_handle, &in_buff,sizeof(in_buff),NULL,&read_ev);
}

void serial_port::on_read()
{
	read_callback();
	start_read();
}

void serial_port::on_write()
{
		write_callback();
}

void serial_port::write(char* buffer, int len)
{
	WriteFile(port_handle,buffer,len,NULL,&write_ev);
}

void serial_port::read_callback()
{
}

void serial_port::write_callback()
{
}