package protocol.commands.types.chatmessage;

import protocol.commands.CommandType;
import protocol.commands.request.Request;

public class ChatMessageRequest extends Request {
    private String message;

    public ChatMessageRequest() {
        super(CommandType.COMMAND_CHATMESSAGE);
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
