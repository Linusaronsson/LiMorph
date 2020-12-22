-- LIMORPH_LUA R"lua(

local f2 = CreateFrame("Frame")
f2:RegisterEvent('UPDATE_SHAPESHIFT_FORM');

function player_shapeshift_event()
	MorphTransparentShapeshift()
end

f2:SetScript("OnEvent", player_shapeshift_event)

-- )lua";