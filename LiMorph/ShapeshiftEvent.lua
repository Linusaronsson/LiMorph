R"V0G0N(

local f2 = CreateFrame("Frame")

f2:RegisterEvent('UPDATE_SHAPESHIFT_FORM');

last_id = -1
n_same_id = 0

function player_shapeshift_event_()
	local id = GetShapeshiftFormID()
	local form = GetShapeshiftForm()
	--print(id)
	print(form)

	if last_id == form then
		n_same_id = n_same_id + 1
	end


	if id ~= 0 and n_same_id == 1 then
		MorphShapeshift(id)
		n_same_id = 0
		last_id = -1
	elseif id == 0 and n_same_id == 2 then
		MorphShapeshift(id)
		n_same_id = 0
		last_id = -1
	else
		last_id = form
	end
	
end

function player_shapeshift_event()
	last_id = -1
	player_shapeshift_event_()
end


f2:SetScript("OnEvent", player_shapeshift_event_)



)V0G0N";
