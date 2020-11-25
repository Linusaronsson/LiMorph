R"V0G0N(

local f2 = CreateFrame("Frame")

f2:RegisterEvent('UPDATE_SHAPESHIFT_FORM');
--last_id = -1
function player_shapeshift_event()
	local id = GetShapeshiftFormID()
    --if id ~= last_id then
	MorphShapeshift(id)
	--end
	--last_id = id
end

f2:SetScript("OnEvent", player_shapeshift_event)



)V0G0N";
