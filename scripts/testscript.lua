print(enumerate_midi_ports())

local midi_port = new_midi_port(0);
midi_port:on_status(0x90,function(data1,data2)
	print(string.format("NOTE ON NN:0x%X, VEL:0x%X",data1,data2))
end)

midi_port:on_status(0x80,function(data1,data2)
	print(string.format("NOTE OFF NN:0x%X, VEL:0x%X",data1,data2))
end)