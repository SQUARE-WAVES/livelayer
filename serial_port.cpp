#include "serial_port.h"
#include <iostream>

serial_read_event::serial_read_event(serial_port* emitter):
event(),
home_port(emitter)
{

}

void serial_read_event::handle()
{
	home_port->on_read();
}

serial_write_event::serial_write_event(serial_port* emitter):
event(),
home_port(emitter)
{

}

void serial_write_event::handle()
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
		//TODO::EXCEPTION!
		std::cout << "port creation error \n";
	}

	HANDLE iocp_success = CreateIoCompletionPort(port_handle,looper->get_handle(),1,1);

	if(iocp_success != looper->get_handle())
	{
		//TODO::EXCEPTION!
		std::cout << "serial port iocp add error\n";
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
	std::cout << "serial read complete: " << in_buff <<'\n';
	start_read();
}

void serial_port::on_write()
{
	std::cout << "serial write complete\n";
}

void serial_port::write(char* buffer, int len)
{
	WriteFile(port_handle,buffer,len,NULL,&write_ev);
}