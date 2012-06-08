local socket = require("socket")
local ticks = 0

local timeouts={}

local function set_timeout(delay,data,method)

	print('setting timeout')
	local realtime = ticks+delay
	if(nil == timeouts[realtime] ) then
		timeouts[realtime]  = {}
	end
	
	table.insert(timeouts[realtime] ,{data=data, callback = method})
end

local function do_timeouts()
	if(nil ~= timeouts[ticks]) then
		for k,v in pairs(timeouts[ticks]) do
			v.callback(v.data)
			timeouts[ticks][k] = nil
		end
	end
end



local tameblock = {}
local function tamecall(method)
	
	tameblock.deferdata = {}
	tameblock.deferdata.keys = {}
	tameblock.deferdata.data = {}
	tameblock.done = false
	
	local check_done = function()
		print('checking')
		for k,v in pairs(tameblock.deferdata.keys) do
			if tameblock.deferdata.data[v] == nil then
				print('not done')
				return
			end
		end
		print('done')
		tameblock.done  = true
		
	end
	
	local defer = function(key)
		--log the key
		table.insert(tameblock.deferdata.keys,key)
	
		return function(data)
			tameblock.deferdata.data[key] = data
			check_done()
		end
	end
	
	local await = function(innermethod)
		innermethod(defer)
		return coroutine.yield()
	end
	
	tameblock.co = coroutine.create(method)
	coroutine.resume(tameblock.co,await)	
end



tamecall(function(await)

	local data = await(function(defer)
		set_timeout(20,"cool",defer('fish'))
		set_timeout(25,"lame",defer('horses'))
	end)
	
	print('fish: ' .. data.fish)
	print('horses: ' .. data.horses)
	
end)

while(1) do
	--wait a tick
	socket.select({},{},.1)
	ticks = ticks+1
	--do your shit
	do_timeouts()
	if(tameblock.done) then
		coroutine.resume(tameblock.co,tameblock.deferdata.data)
		tameblock.done = false
	end
end