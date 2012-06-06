local serial = require("serial_section")
local mtof,print_note = require("mtof")
local bit = require("bit")

--constants!
local DEFAULT_CLOCK_OCT = 0x0A
local DEFAULT_CLOCK_DAC_HI,DEFAULT_CLOCK_DAC_LOW = 0x03,0xBF
local DEFAULT_AMP = 0x0F --this is off
local DEFAULT_NOISE = 0x04 --"white" noise at max clock

local chip1_info = {}
local chip2_info = {}

chip1_info.index = 0
chip2_info.index = 1

--as part of this mod you have to open the serial port
assert(serial.open_port("\\\\.\\COM13"))

local chip1 = {}
local chip2 = {}

chip1.init = function()
	local hi,lo = mtof(69)
	print("initing chipone")
	
	for i=0,2 do
		chip1_info[i] = {}
		chip1_info[i].freq_hi = hi
		chip1_info[i].freq_lo = lo
		chip1_info[i].amp = DEFAULT_AMP
		serial.set_freq(0,i,hi,lo)
		serial.set_amp(0,i,DEFAULT_AMP)
	end
	print("done with the settings")
	chip1_info[3] = {}
	chip1_info[3].noise = DEFAULT_NOISE
	chip1_info[3].amp = DEFAULT_AMPS
	serial.set_noise(0,DEFAULT_NOISE)
	serial.set_amp(0,3,DEFAULT_AMP)
	print("done with the sendings")
	
	chip1_info.clock_oct = DEFAULT_CLOCK_OCT
	chip1_info.clock_dac_hi = DEFAULT_CLOCK_DAC_HI
	chip1_info.clock_dac_low = DEFAULT_CLOCK_DAC_LOW
	serial.set_clock(0,DEFAULT_CLOCK_OCT,DEFAULT_CLOCK_DAC_HI,DEFAULT_CLOCK_DAC_LOW)
	print("done with the clock")
	
end

chip2.init = function()
	local hi,lo = mtof(69)

	for i=0,2 do
		chip2_info[i] = {}
		chip2_info[i].freq_hi = hi
		chip2_info[i].freq_lo = lo
		chip2_info[i].amp = DEFAULT_AMP
		serial.set_freq(1,i,hi,lo)
		serial.set_amp(1,i,DEFAULT_AMP)
	end
	
	chip2_info[3] = {}
	chip2_info[3].noise = DEFAULT_NOISE
	chip2_info[3].amp = DEFAULT_AMPS
	serial.set_noise(1,DEFAULT_NOISE)
	serial.set_amp(1,3,DEFAULT_AMP)
	
	chip2_info.clock_oct = DEFAULT_CLOCK_OCT
	chip2_info.clock_dac_hi = DEFAULT_CLOCK_DAC_HI
	chip2_info.clock_dac_low = DEFAULT_CLOCK_DAC_LOW
	serial.set_clock(1,DEFAULT_CLOCK_OCT,DEFAULT_CLOCK_DAC_HI,DEFAULT_CLOCK_DAC_LOW)
	print("done with chip 2")
end

local function drop_oct(chip_info)
	if(chip_info.clock_oct > 0x8) then
		chip_info.clock_oct = chip_info.clock_oct - 1
		serial.set_clock(chip_info.index,chip_info.clock_oct,chip_info.clock_dac_hi,chip_info.clock_dac_low)
	end
end

local function raise_oct(chip_info)
	if(chip_info.clock_oct < 0xA) then
		chip_info.clock_oct = chip_info.clock_oct + 1
		serial.set_clock(chip_info.index,chip_info.clock_oct,chip_info.clock_dac_hi,chip_info.clock_dac_low)
	end
end

local function change_note(chip_info,channel,note)
	local hi,lo = mtof(note)
	chip_info[channel].freq_hi = hi
	chip_info[channel].freq_lo = lo
	serial.set_freq(chip_info.index,channel,hi,lo)
end

local function change_amp(chip_info,channel,amp)
	local truncamp = bit.band(amp,0x0F)
	chip_info[channel].amp = truncamp
	serial.set_amp(chip_info.index,channel,truncamp)
end

local function change_noise(chip_info,noise)
	local truncnoise = bit.band(noise,0x0F)
	chip_info[3].noise = truncnoise
	serial.set_nose(chip_info.index,truncnoise)
end

chip1.drop_oct = function()
	drop_oct(chip1_info)
end

chip2.drop_oct = function()
	drop_oct(chip2_info)
end

chip1.raise_oct = function()
	raise_oct(chip1_info)
end

chip2.raise_oct = function()
	raise_oct(chip2_info)
end

chip1.note = function(channel,note)
	change_note(chip1_info,channel,note)
end

chip2.note = function(channel,note)
	change_note(chip2_info,channel,note)
end

chip1.amp = function(channel,amp)
	change_amp(chip1_info,channel,amp)
end

chip2.amp = function(channel,amp)
	change_amp(chip2_info,channel,amp)
end

chip1.noise = function(noise)
	change_noise(chip1_info,noise)
end

chip2.noise = function(noise)
	change_noise(chip2_info,noise)
end


return chip1,chip2