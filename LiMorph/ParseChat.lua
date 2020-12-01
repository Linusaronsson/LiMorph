-- LIMORPH_LUA R"lua(

local original_ChatEdit_SendText = ChatEdit_SendText;

function ParseChatLua(editBox, addHistory)
    local text = editBox:GetText(); 
    if (strsub(text, 1, 1) == "." and strlen(text) > 4) then
        ParseChat(text)
    else
        original_ChatEdit_SendText(editBox, addHistory)
    end
end

ChatEdit_SendText = ParseChatLua;

-- )lua";
