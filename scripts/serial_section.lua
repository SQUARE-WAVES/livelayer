local bit = require("bit")
require("pack") --for string.pack

local p --this is the port

local function write_to_port(...)
	local send = string.pack(...)
	p:write(send)
end

local SET_CLOCK = 0
local SET_AMP = 1
local SET_FREQ = 2
local SET_NOISE = 3
local WRITE_CHIP = 4

local modtable = {}

modtable.open_port = function(port_name)
	
	p = assert(new_serial_port(port_name))
	return true
end

modtable.set_freq = function(chip,channel,freqhi,freqlow)
	write_to_port("bbbbbb",0x05,SET_FREQ,chip,channel,freqhi,freqlow)
end

modtable.set_amp = function(chip,channel,amp)
	write_to_port("bbbbb",0x04,SET_AMP,chip,channel,amp)
end

modtable.set_noise = function(chip,noise)
	write_to_port("bbbb",0x03,SET_NOISE,chip,noise)
end

modtable.set_clock = function(chip,oct,dachi,daclow)
	write_to_port("bbbbbb",0x05,SET_CLOCK,chip,oct,dachi,daclow)
end

modtable.write_chip = function (chip,data)
	write_to_port("bbbb",0x3,WRITE_CHIP,chip,data)
end

return modtable


