-- LIMORPH_LUA R"lua(


local InvTypeToSlot = {
	INVTYPE_HEAD = INVSLOT_HEAD, -- 1
	INVTYPE_SHOULDER = INVSLOT_SHOULDER, -- 3
	INVTYPE_BODY = INVSLOT_BODY, -- 4
	INVTYPE_CHEST = INVSLOT_CHEST, -- 5
	INVTYPE_ROBE = INVSLOT_CHEST, -- 5 (cloth)
	INVTYPE_WAIST = INVSLOT_WAIST, -- 6
	INVTYPE_LEGS = INVSLOT_LEGS, -- 7
	INVTYPE_FEET = INVSLOT_FEET, -- 8
	INVTYPE_WRIST = INVSLOT_WRIST, -- 9
	INVTYPE_HAND = INVSLOT_HAND, -- 10
	INVTYPE_CLOAK = INVSLOT_BACK, -- 15
	INVTYPE_2HWEAPON = INVSLOT_MAINHAND, -- 16
	INVTYPE_WEAPON = INVSLOT_MAINHAND, -- 16
	INVTYPE_WEAPONMAINHAND = INVSLOT_MAINHAND, -- 16
	INVTYPE_WEAPONOFFHAND = INVSLOT_OFFHAND, -- 17
	INVTYPE_HOLDABLE = INVSLOT_OFFHAND, -- 17
	INVTYPE_RANGED = INVSLOT_RANGED, -- 18
	INVTYPE_THROWN = INVSLOT_RANGED, -- 18
	INVTYPE_RANGEDRIGHT = INVSLOT_RANGED, -- 18
	INVTYPE_SHIELD = INVSLOT_OFFHAND, -- 17
	INVTYPE_TABARD = INVSLOT_TABARD, -- 19
}

local GearSlots = {
	INVSLOT_HEAD, -- 1
	INVSLOT_SHOULDER, -- 3
	INVSLOT_BODY, -- 4
	INVSLOT_CHEST, -- 5
	INVSLOT_WAIST, -- 6
	INVSLOT_LEGS, -- 7
	INVSLOT_FEET, -- 8
	INVSLOT_WRIST, -- 9
	INVSLOT_HAND, -- 10
	INVSLOT_BACK, -- 15
	INVSLOT_TABARD, -- 19
}

local SlotNames = {
	[INVSLOT_HEAD] = "head", -- 1
	[INVSLOT_SHOULDER] = "shoulder", -- 3
	[INVSLOT_BODY] = "shirt", -- 4
	[INVSLOT_CHEST] = "chest", -- 5
	[INVSLOT_WAIST] = "belt", -- 6
	[INVSLOT_LEGS] = "legs", -- 7
	[INVSLOT_FEET] = "feet", -- 8
	[INVSLOT_WRIST] = "wrist", -- 9
	[INVSLOT_HAND] = "hands", -- 10
	[INVSLOT_BACK] = "cloak", -- 15
	[INVSLOT_MAINHAND] = "mainhand", -- 16
	[INVSLOT_OFFHAND] = "offhand", -- 17
	[INVSLOT_RANGED] = "ranged", -- 18
	[INVSLOT_TABARD] = "tabard", -- 19
}

function MorphItemCustom(slotID, itemID, versionID) 
	if IsAltKeyDown() then
		MorphItem(slotID, itemID, versionID)
	end
end

function MorphEnchantCustom(slotID, enchantID) 
	if IsAltKeyDown() then
		MorphEnchant(slotID, enchantID)
	end
end



function GetItemInfo_(item)
	-- try to preserve item link if we receive one
	if type(item) == "string" then
		local itemID = tonumber(item:match("item:(%d+)"))
		local equipLoc = select(9, GetItemInfo(itemID))
		return itemID, item, equipLoc
	else
		local itemLink, _, _, _, _, _, _, equipLoc = select(2, GetItemInfo(item))
		return item, itemLink, equipLoc
	end
end

hooksecurefunc("HandleModifiedItemClick", function(item)
	if IsAltKeyDown() then
		local itemID, itemLink, equipLoc = GetItemInfo_(item)
		local slotID = InvTypeToSlot[equipLoc]
		if slotID then
			MorphItemCustom(slotID, itemID, 0)
		end
	end
	--print(itemID)
	--print(itemLink)
	--print(equipLoc)

end)

function MorphTransmogSet() -- retail
	local setID = WardrobeCollectionFrame.SetsCollectionFrame.selectedSetID
	local setInfo = C_TransmogSets.GetSetInfo(setID)

	for _, v in pairs(WardrobeSetsDataProviderMixin:GetSortedSetSources(setID)) do
		local source = C_TransmogCollection.GetSourceInfo(v.sourceID)
		local slotID = C_Transmog.GetSlotForInventoryType(v.invType)
		--print(slotID)
		--print(source.itemID)
		MorphItemCustom(slotID, source.itemID, source.itemModID)
		--morph.item("player", CM.SlotNames[slotID], source.itemID, source.itemModID)
	end

end

local DualWieldSlot = {
	[INVSLOT_MAINHAND] = INVSLOT_OFFHAND,
	[INVSLOT_OFFHAND] = INVSLOT_MAINHAND,
}

function GetDualWieldSlot(slot)
	if DualWieldSlot[slot] and IsDualWielding() then
		lastWeaponSlot = DualWieldSlot[lastWeaponSlot]
		return lastWeaponSlot
	else
		return slot
	end
end

function MorphItemBySource(source)
	local slotID = C_Transmog.GetSlotForInventoryType(source.invType)
	slotID = GetDualWieldSlot(slotID)
	MorphItemCustom(slotID, source.itemID, source.itemModID)
end

function MorphTransmogItem(frame) -- retail
	local loc = WardrobeCollectionFrame.ItemsCollectionFrame.transmogLocation
	local visualID = frame.visualInfo.visualID
	if loc:IsIllusion() then
		MorphEnchantCustom(loc.slotID, visualID)
	elseif loc:IsAppearance() then
		local sources = WardrobeCollectionFrame_GetSortedAppearanceSources(visualID)
		for idx, source in pairs(sources) do
			-- get the index the arrow is pointing at
			if idx == WardrobeCollectionFrame.tooltipSourceIndex then
				MorphItemBySource(source)
			end
		end
	end
end

local f7 = CreateFrame("Frame")

local active

if IsAddOnLoaded("Blizzard_Collections") then
	f7:InitWardrobe()
else
	f7:RegisterEvent("ADDON_LOADED")
end

function f7:InitWardrobe()
	-- only load once the appearances tab is opened
	WardrobeCollectionFrame:HookScript("OnShow", function(frame)
		if active then
			return
		end
		active = true
		ItemsCollection = WardrobeCollectionFrame.ItemsCollectionFrame

		-- item sets
		WardrobeCollectionFrame.SetsCollectionFrame.Model:HookScript("OnMouseUp", MorphTransmogSet)
		for _, model in pairs(ItemsCollection.Models) do
			model:HookScript("OnMouseUp", MorphTransmogItem)
		end
	end)
end

f7:SetScript("OnEvent", function(self, event, arg1)
	if event == "ADDON_LOADED" and arg1 == "Blizzard_Collections" then
		self:InitWardrobe()
		self:UnregisterEvent(event)
	end
end)



-- )lua";