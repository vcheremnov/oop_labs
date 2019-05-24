package protocol.commands.types.chatmessage;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseError;

public class ChatMessageResponseError extends ResponseError {
    public ChatMessageResponseError() {
        super(CommandType.COMMAND_CHATMESSAGE);
    }
}
