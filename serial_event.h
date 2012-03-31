#ifndef SERIAL_EVENT_DOT_H
#define SERIAL_EVENT_DOT_H

class serial_event
{
	protected:
		uint8_t* bytes;
		int len;
		
	public:
		serial_event(uint8_t* message,int len);
		~serial_event();

		int size();
		uint8_t* getbytes();
};


#endif