-- LIMORPH_LUA R"lua(

function MorphMountCustom(mountID)
	if IsAltKeyDown() then
		local _, spellID = C_MountJournal.GetMountInfoByID(mountID)
		local displayID = C_MountJournal.GetMountInfoExtraByID(mountID)
		if not displayID then
			local multipleIDs = C_MountJournal.GetMountAllCreatureDisplayInfoByID(mountID)
			displayID = multipleIDs[random(#multipleIDs)].creatureDisplayID
		end
		MorphMount(displayID)
	end
end

function MorphMountModelScene()
	local mountID = MountJournal.selectedMountID
	MorphMountCustom(mountID)
end

function MorphMountScrollFrame(frame)
	local mountID = select(12, C_MountJournal.GetDisplayedMountInfo(frame.index))
	MorphMountCustom(mountID)
end

local f9 = CreateFrame("Frame")
local active

function f9:CreateUnlockButton()
	local btn = CreateFrame("Button", nil, MountJournal, "UIPanelButtonTemplate")
	btn:SetPoint("LEFT", MountJournal.MountCount, "RIGHT", 5, 0) -- topleft corner of the frame
	btn:SetWidth(100)
	btn:SetText(UNLOCK)
	btn:SetScript("OnClick", function(frame)
		self:UnlockMounts()
		frame:Hide()
	end)
end

function f9:UnlockMounts()
	local mountIDs = C_MountJournal.GetMountIDs()
	local searchMountIDs, activeSearch = {}
	-- sort alphabetically
	sort(mountIDs, function(a, b)
		local name1, _, _, _, _, _, isFavorite1 = C_MountJournal.GetMountInfoByID(a)
		local name2, _, _, _, _, _, isFavorite2 = C_MountJournal.GetMountInfoByID(b)
		-- show favorites first, cant favorite an uncollected mount btw
		if isFavorite1 ~= isFavorite2 then
			return isFavorite1
		else
			return name1 < name2
		end
	end)

	local function GetActiveMountIDs()
		return activeSearch and searchMountIDs or mountIDs
	end

	-- replace api, pray nothing explodes
	function C_MountJournal.GetNumDisplayedMounts()
		return #GetActiveMountIDs()
	end

	function C_MountJournal.GetDisplayedMountInfo(index)
		local ids = GetActiveMountIDs()
		local args = {C_MountJournal.GetMountInfoByID(ids[index])}
		args[5] = true -- fake isUsable
		return unpack(args)
	end

	-- set mount count fontstring
	hooksecurefunc("MountJournal_UpdateMountList", self.UpdateMountCount)
	hooksecurefunc(MountJournal.ListScrollFrame, "update", self.UpdateMountCount) -- OnMouseWheel
	self.UpdateMountCount()

	-- roll our own search function since default search (server side) is restricted to the normal subset
	MountJournal.searchBox:HookScript("OnTextChanged", function(self)
		local text = self:GetText():trim():lower()
		if #text > 0 then
			wipe(searchMountIDs)
			activeSearch = true
			for _, v in pairs(mountIDs) do
				-- should probably optimize this with a cache
				if C_MountJournal.GetMountInfoByID(v):lower():find(text) then
					tinsert(searchMountIDs, v)
				end
			end
			-- dont wait for MOUNT_JOURNAL_SEARCH_UPDATED
			MountJournal_UpdateMountList()
		else
			activeSearch = false
		end
	end)

	local function ClearSearch()
		wipe(searchMountIDs)
		activeSearch = false
	end
	MountJournal.searchBox:HookScript("OnHide", ClearSearch)
	MountJournal.searchBox.clearButton:HookScript("OnClick", ClearSearch)
	MountJournal_FullUpdate(MountJournal)
end

function f9.UpdateMountCount()
	local ids = C_MountJournal.GetMountIDs()
	MountJournal.MountCount.Count:SetText(#ids)
end

function f9:InitMountJournal()
	-- only load once the mounts tab is opened
	MountJournal:HookScript("OnShow", function(frame)
		if active then
			return
		end
		active = true
		self:CreateUnlockButton()
		-- modelscene
		MountJournal.MountDisplay.ModelScene:HookScript("OnMouseUp", MorphMountModelScene)
		-- scrollframe buttons
		for _, button in pairs(MountJournal.ListScrollFrame.buttons) do
			button:HookScript("OnClick", MorphMountScrollFrame)
		end
	end)
end


if IsAddOnLoaded("Blizzard_Collections") then
    f9:InitMountJournal()
else
    f9:RegisterEvent("ADDON_LOADED")
    f9:SetScript("OnEvent", function(self, event, addon)
        if addon == "Blizzard_Collections" then
            self:InitMountJournal()
            self:UnregisterEvent(event)
        end
    end)
end

-- )lua";