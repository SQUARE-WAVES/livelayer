#include <WinSock2.h>
#include <iostream>
#include "udp_port.h"

udp_read_event::udp_read_event(udp_port* port)
{
	this->home_port = port;
}

void udp_read_event::handle(int iobytes)
{
	home_port->on_read(iobytes);
}

bool udp_port::initialize()
{
	WSADATA data;
	WORD version = 0x101; 
	int ret = 0;  

	version = (MAKEWORD(2, 2)); 
	ret = WSAStartup(version, &data); 

	if(ret != 0) 
	{  
		ret = WSAGetLastError(); 
        
		if(ret == WSANOTINITIALISED) 
		{  
			return false;
		}
	}

	return true;
}

udp_port::udp_port(unsigned int inaddr,int portnum,event_loop* looper):
theloop(looper),
ev(this)
{
	memset(buffer,0x00,sizeof(buffer));

	HANDLE iocp_handle = theloop->get_handle();

	sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	if(sock == INVALID_SOCKET)  
  { 
		std::cout<<"socket() failed, Err: " << WSAGetLastError() << '\n'; 
	  throw std::exception("create socket failed");
  }

	SOCKADDR_IN sock_in_addr;
	sock_in_addr.sin_family = AF_INET; 
  sock_in_addr.sin_addr.s_addr = htonl(inaddr); 
  sock_in_addr.sin_port = htons(portnum);

	int ret = bind(sock,(struct sockaddr*)&sock_in_addr,sizeof(sock_in_addr)); 

  if (ret == SOCKET_ERROR)  
  { 
		std::cout << " ind() port: "<<portnum<<" failed, Err: "<<WSAGetLastError()<<'\n'; 
  }

	CreateIoCompletionPort((HANDLE)sock,iocp_handle,event_loop::EVENT_KEY,1);

	int b = ReadFile ((HANDLE)sock,&buffer,sizeof(buffer),NULL,&ev);  
    
	if (!b && GetLastError () != ERROR_IO_PENDING)  
	{  
		printf("ReadFile Failed\n");  
		throw std::exception("initial read failed");
	}
}

bool udp_port::send_to(unsigned int addr, int port, uint8_t* buffer,int len)
{
	SOCKADDR_IN sock_in_addr;
	sock_in_addr.sin_family = AF_INET;
	sock_in_addr.sin_addr.s_addr = htonl(addr); 
  sock_in_addr.sin_port = htons(port);

	int ret = sendto(sock,(const char*)buffer,len,0,(struct sockaddr*)&sock_in_addr,sizeof(sock_in_addr));
	if (ret == SOCKET_ERROR) 
	{
    std::cout<<"send error: " << WSAGetLastError() <<'\n';
		return false;
	}

	return true;
}


void udp_port::on_read(int read_length)
{
	read_callback(buffer,read_length);
	memset(buffer,0x00,sizeof(buffer));

	int b = ReadFile ((HANDLE)sock,&buffer,sizeof(buffer),NULL,&ev);  
    
	if (!b && GetLastError () != ERROR_IO_PENDING)  
	{  
		printf("ReadFile Failed\n");  
		throw std::exception("next read failed");
	}
}

void udp_port::read_callback(uint8_t* buffer, int length)
{
}