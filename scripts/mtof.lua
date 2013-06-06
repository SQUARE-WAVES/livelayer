local bit=require("bit")

local mtoftable = {}

function round (x)
  if x >= 0 then
    return math.floor (x + 0.5)
  end  -- if positive

  return math.ceil (x - 0.5)
end -- function round

for i=0,127 do
	local freq = 440*math.pow(2,(i-69)/12)
	local settings = round((4000000/(32*freq)))
	local tenbit = bit.band(settings,0x3FF)
	mtoftable[i] = {tenbit}
	--mtoftable[i] = {bit.rshift(settings,4),bit.band(settings,0xF)}
end

print('made the mtof table')
print(mtoftable[60])

local mtoffunction = function(midi)
	return unpack(mtoftable[midi])
end

local function print_note(nn)
	local f = 440*math.pow(2,(nn-69)/12)
	print(string.format("freq: %d high: 0x%X, low: 0x%X",f,unpack(mtoftable[nn])))
end


local make_tabs = function()

	file = io.open("mtabs.txt","w");
	
	file:write("hi bytes\n\n");
	for i = 0,127 do
		local hi,low = mtoffunction(i)
		file:write(string.format("0x%X,\n",hi))
	end
	
	file:close();
	
end

make_tabs();






return mtoffunction, print_note

--[[start tests]

local function test_mtoffunction()
1
	for i,v in pairs(mtoftable) do
		print(string.format("nn: %d settinghi 0x%X, settinglow: 0x%X",i,v[0],v[1]))
	end

	local hi,lo = mtoffunction(69)
	assert(hi == 0x01)
	assert(lo == 0x1C)
	
	print("mtoffunction test passed")
end



-end tests]]--