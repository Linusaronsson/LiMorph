R"V0G0N(

local f = CreateFrame("Frame")

f:RegisterEvent('PLAYER_MOUNT_DISPLAY_CHANGED');

function player_mount_event()
	if IsMounted() and not UnitOnTaxi("player") then
		MorphMount()
	end
end

f:SetScript("OnEvent", player_mount_event)

)V0G0N";
