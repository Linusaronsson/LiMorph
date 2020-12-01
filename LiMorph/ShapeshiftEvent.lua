-- LIMORPH_LUA R"lua(

local f2 = CreateFrame("Frame")
f2:RegisterEvent('UPDATE_SHAPESHIFT_FORM');

function player_shapeshift_event_()
	local id = GetShapeshiftFormID()
	--local form = GetShapeshiftForm()
	MorphShapeshift(id, 0)
end

function player_shapeshift_event()
	local id = GetShapeshiftFormID()
	MorphShapeshift(id, 1)
	--player_shapeshift_event_()
end

f2:SetScript("OnEvent", player_shapeshift_event_)


-- )lua";