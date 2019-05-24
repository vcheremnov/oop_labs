package protocol.commands.request;

import protocol.Message;
import protocol.MessageType;
import protocol.commands.CommandType;

public abstract class Request extends Message {
    private String commandType;
    private String sessionID;

    public Request(CommandType commandType) {
        super(MessageType.MESSAGE_REQUEST);
        this.commandType = commandType.toString();
    }

    public String getCommandType() {
        return commandType;
    }

    public String getSessionID() {
        return sessionID;
    }

    public void setSessionID(String sessionID) {
        this.sessionID = sessionID;
    }
}
