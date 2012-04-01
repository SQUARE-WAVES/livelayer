p = new_serial_port("COM3")
midi = new_midi_port(8)

p:onread(function(str)
	print("LOOPBACK: " .. str)
end)


midi:MIDIin(0x90,function(status,note,vel)
	p:write(string.format("n: 0x%X 0x%X",note,vel))
end)
--[[

chip1,chip2 = dofile('chips.lua')
midi_port = new_midi_port(8)

chip1.init()
chip2.init()

--setup the note on and note off handlers
for i=0,2 do
	midi_port:MIDIin(0x90+i,function(status,note,vel)
		chip1.note(0+i,note)
		if(vel ~=0) then
			chip1.amp(0+i,0x00)
		else
			chip1.amp(0+i,0x0F)
		end
	end) 

	midi_port:MIDIin(0x80+i,function(status,note,val)
		chip1.amp(0+i,0x0F)
	end)
	
end

event_handlers_table = {}
event_handlers_table.mousemove_handler = function(x,y)
	print('move:',x,y)
	chip1.amp(0,0x0F)
	chip1.amp(1,0x0F)
	chip1.amp(2,0x0F)
end



event_handlers_table = {}
midi_port = new_midi_port(8)

local count = 0

midi_port:ignore_types(true,false,true)

midi_port:MIDIin(0x90,function(status,note,vel)
	print(string.format("midiin: 0x%X 0x%X 0x%X",status,note,vel))
end)

midi_port:MIDIin(0xF8,function(status,note,vel)
	count = count+1
	if count == 96 then
		print('measure!')
		count = 0
	end
end)

serial_port = new_serial_port("COM3")

serial_port:onread(function(str)
	print(str)
end)

function register_keyup_event(method)
	event_handlers_table['keyup_handler'] = method
end

function register_keydown_event(method)
	event_handlers_table['keydown_handler'] = method
end

function register_mouseup_event(method)
	event_handlers_table["mouseup_handler"] = method
end

function register_mousedown_event(method)
	event_handlers_table["mousedown_handler"] = method
end

function register_mousemove_event(method)
	event_handlers_table["mousemove_handler"] = method
end

register_keyup_event(function(sym)
	print('key up:',sym)
end)

register_keydown_event(function(sym)
	print('key down:',sym)
end)

register_mousedown_event(function(x,y,button)
	print('mouse down:',x,y,button)
end)

register_mouseup_event(function(x,y,button)
	print('mouse up:',x,y,button)
end)

register_mousemove_event(function(x,y)
	print('move:',x,y)
end)
]]--