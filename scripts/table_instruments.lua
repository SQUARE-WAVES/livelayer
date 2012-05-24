local pitchtable = {}

local function init_pitchtable()
	for i=1,64 do
		pitchtable[i]={}
		pitchtable[i].note_offset = 0
		pitchtable[i].jmp = 0
	end
end

function pitchtable:set_val(index,offset)
	assert(index>1 and index <65,"index out of range")
	self[index[ .note_offset= offset
end

function pitchtable:set_jmp(index,jump)
	assert(index>1 and index <65,"index out of range")
	assert(jump>1 and jump<65,"jump out of range")
	self[index].jmp = jump
end

function new_sqrinstr = function(chip, channel)

	assert(channel > 0 and channel<2,"channel is out of range for a square instrument")

	local instr = {}
	instr.chip = chip
	instr.channel = channel
	instr.pitch_offset = 0 --this is where it is now
	instr.pitch_startpoint= 0 --this is where it starts on a new note
	
	instr:note_on = function(note_number)
	end
	
	instr:note_off = function(note_number)
	end
	
	instr:tick = function()
	


end