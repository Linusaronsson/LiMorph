-- LIMORPH_LUA R"lua(

local original_ChatEdit_SendText = ChatEdit_SendText;

function ParseChatLua(editBox, addHistory)
    --print("wtf?")
    local text = editBox:GetText(); 
    if (strsub(text, 1, 1) == "." and strlen(text) > 3) then
        ParseChat(text)
        CustomMessage()
    else
        original_ChatEdit_SendText(editBox, addHistory)
    end
end

ChatEdit_SendText = ParseChatLua;

-- )lua";
