--[[---------------------------------------------------------------------------
--
--		this is a monophonic synth voicer, it translates note on and note off
--		messages into controls for an "amp/freq" type synth, basically you supply
--		it with 3 functions, start voice, change voice, and end voice
--		for now it will have last note priority, later we will add low note
--
---------------------------------------------------------------------------]]--
local voicer = {}

function new_voicer_lastnote(s_voice,c_voice,e_voices)

	assert("function" == type(s_voice),"first argument must be a function")
	assert("function" == type(c_voice),"second argument must be a function")
	assert("function" == type(e_voice),"third argument must be a function")

	--the object itself, so it exists in the closure of all functions
	local voicer = {}

	--PRIVATE stuff, stuff found only in the closure of the functions--
	local notes_on = {}
	--right now we don't check to see if the new note is the same as the 
	--present note, that means it will call change note to the same note
	--I'm not sure if that is a good or bad behavior.
	
	local function legato_note(note,vel) --a new note when there is allready one held
		--first see if this note is allready on, if it is yank it out
		local index = -1
		for i=1,#notes_on do
			index = ((note==notes_on[i]) and i) or index
		end
		if(-1 ~= index) then
			table.remove(notes_on,index)
		end

		table.insert(notes_on,note)
		c_voice(note,vel)
	end

	local function trigger_note(note,vel) --this means a new note when none are playing
		table.insert(notes_on,note)
		voicer.note_on = legato_note
		s_voice(note,vel)
	end

	function voicer.note_off(note,vel)
		--ok its the present note, we will need to change voices
		if(note == notes_on[#notes_on]) then 
			table.remove(notes_on,#notes_on)
			--if we still have notes on
			if(0 ~= #notes_on) then
				c_voice(notes_on[#notes_on],vel)
			else
				e_voice(note,vel)
				voicer.note_on = trigger_note
			end
		else --just take it out
			for i,v in ipairs(notes_on) do
				if v==note then 
					table.remove(notes_on,i) 
				end
			end
		end
	end

	--for debugging
	function voicer.print_notes()
		for i,v in ipairs(notes_on) do
			print(string.format("%i: %i",i,v))
		end
	end

	--assign the proper functions
	voicer.note_on = trigger_note

	--and give them a voicer
	return voicer	
end


function new_voicer_lownote(s_voice,c_voice,e_voice)

	assert("function" == type(s_voice),"first argument must be a function")
	assert("function" == type(c_voice),"second argument must be a function")
	assert("function" == type(e_voice),"third argument must be a function")

	--the object itself, so it exists in the closure of all functions
	local voicer = {}

	--PRIVATE stuff, stuff found only in the closure of the functions--
	local notes_on = {} --optimization note:: init this table to prevent allocation bugs
	local present_note = -1 --this is for low priority and such
	
	--right now we don't check to see if the new note is the same as the 
	--present note, that means it will call change note to the same note
	--I'm not sure if that is a good or bad behavior.
	
	local function legato_note(note,vel) --a new note when there is allready one held
		notes_on[note] = true
		present_note = note
		c_voice(note,vel)
	end

	local function trigger_note(note,vel) --this means a new note when none are playing
		notes_on[note] = true
		present_note = note
		voicer.note_on = legato_note
		s_voice(note,vel)
	end

	function voicer.note_off(note,vel)
		notes_on[note] = false
		--if it was the voice we were hearing
		--we have to figure out then next note
		if(note == present_note) then
			for i=0,127 do
				if(notes_on[i] == true) then
					present_note = i
					c_voice(i,vel)
					return
				end
			end
			--if we didn't find one
			present_note = -1
			e_voice(note,vel)
			voicer.note_on = trigger_note
		end
	end

	function voicer.present_note()
		return present_note
	end

	--assign the proper functions
	voicer.note_on = trigger_note

	--and give them a voicer
	return voicer	
end

voicer.low = new_voicer_lownote
voicer.last = new_voicer_lastnote

return voicer
