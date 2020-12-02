-- LIMORPH_LUA R"lua(

local f = CreateFrame("Frame")

f:RegisterEvent('PLAYER_MOUNT_DISPLAY_CHANGED');

function player_mount_event()
	--print("mount event reached")
	if IsMounted() and not UnitOnTaxi("player") then
		MorphMount_()
	end
end

f:SetScript("OnEvent", player_mount_event)

-- )lua";