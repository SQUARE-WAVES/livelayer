local midi_port = {}
midi_port.new = function(portnum)
	local ud,err = livelayer.new_midi_port()
	if(nil == ud) then
		return false, err
	end
	
	local port = {}
	port.MIDIin = function(status,method)
		ud[status] = method
	end
	
end



local port = midi_port.new(8)
port.MIDIin(0x90,function(status,note,vel)
	print(string.format('NOTE ON status: 0x%X note: 0x%X vel: 0x%X',status,note,vel)
end)

port.MIDIin(0x80,function(status,note,vel)
	print(string.format('NOTE OFF status: 0x%X note: 0x%X vel: 0x%X',status,note,vel)
end)