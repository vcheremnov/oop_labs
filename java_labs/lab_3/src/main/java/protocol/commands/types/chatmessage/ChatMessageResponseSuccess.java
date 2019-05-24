package protocol.commands.types.chatmessage;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseSuccess;

public class ChatMessageResponseSuccess extends ResponseSuccess {
    public ChatMessageResponseSuccess() {
        super(CommandType.COMMAND_CHATMESSAGE);
    }
}
