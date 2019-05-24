package protocol.commands.response;

import protocol.Message;
import protocol.MessageType;
import protocol.commands.CommandType;

public abstract class Response extends Message {
    private String commandType;
    private String status;

    public Response(CommandType commandType, ResponseStatus status) {
        super(MessageType.MESSAGE_RESPONSE);
        this.commandType = commandType.toString();
        this.status = status.toString();
    }

    public String getCommandType() {
        return commandType;
    }

    public String getStatus() {
        return status;
    }
}
