local pitchtable = {}
local newvoicer = require("voicer")

local function init_pitchtable()
	for i=1,64 do
		pitchtable[i]={}
		pitchtable[i].note_offset = 0
		pitchtable[i].jmp = 0
	end
end

init_pitchtable()

pitchtable.set_val = function(self,index,offset)
	self[index].note_offset= offset
end

pitchtable.set_jmp = function(self,index,jump)
	self[index].jmp = jump
end

pitchtable.next_index = function(self,index)
	local jmp = self[index].jmp
	
	if(jmp == 0) then
		return index+1
	else
		return jmp
	end
end

local new_sqrinstr = function(chip, channel)

	assert(channel > 0 and channel<2,"channel is out of range for a square instrument")
	local instr = {}
	
	--state (to be kept in the function's closures)
	pitch_offset = 1 --this is where it is now
	pitch_startpoint= 1 --this is where it starts on a new note
	ticks = 0;
	latest_note = -1;
	
	instr.adjust_note = function(self,note)
		return pitchtable[pitch_offset].note_offset + note
	end
	
	--start a voice
	local function s_voice(note,vel)
		chip.note(channel, instr:adjust_note(note))
		chip.amp(channel, 0x00)
	end
	
	--change a voice (legato)
	local function c_voice(note,vel)
		chip.note(channel, instr:adjust_note(note))
	end
	
	--end a voice
	local function e_voice(note,vel)
		chip.amp(channel,0x0F)
		instr.pitch_offset = instr.pitch_startpoint
	end
	
	local voicer = newvoicer.low(s_voice,c_voice,e_voice);
	
	instr.tick = function(self)
		pitch_offset = pitchtable:next_index(pitch_offset)
		local note = voicer.present_note()
		chip.note(channel, instr:adjust_note(note))
	end
	
	instr.note_on = function(self,note,vel)
		if(vel ~=0) then
			voicer.note_on(note,vel)
		else
			voicer.note_off(note,vel)
		end
	end
	
	instr.note_off = function(self,note,vel)
		voicer.note_off(note,vel)
	end
	
	instr.set_pitch_index = function(self,index)
		pitch_startpoint = index
	end
	
	return instr
	
end


--now for tests
fakechip = {}
fakechip.note = function(channel,note)
	print(string.format("NOTE SET %d %d \n",channel,note))
end

fakechip.amp = function(channel,amp)
	print(string.format("AMP SET %d 0x%X \n",channel,amp))
end

local instrument = new_sqrinstr(fakechip,1)


pitchtable:set_val(1,0)
pitchtable:set_val(2,1)
pitchtable:set_val(3,2)

pitchtable:set_jmp(3,1)

instrument:note_on(40,64)
instrument:tick()
instrument:tick()
instrument:note_on(30,64)
instrument:tick()
instrument:tick()
instrument:note_off(40,0)
instrument:tick()
instrument:tick()
instrument:note_off(30,0)








