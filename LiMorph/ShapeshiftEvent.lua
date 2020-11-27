R"V0G0N(

local f2 = CreateFrame("Frame")

f2:RegisterEvent('UPDATE_SHAPESHIFT_FORM');
last_id = -1
function player_shapeshift_event_()
	local id = GetShapeshiftFormID()
   -- if id ~= last_id then
		MorphShapeshift(id)
--	end
	last_id = id
end

function player_shapeshift_event()
	last_id = -1
	player_shapeshift_event_()
end


f2:SetScript("OnEvent", player_shapeshift_event_)



)V0G0N";
