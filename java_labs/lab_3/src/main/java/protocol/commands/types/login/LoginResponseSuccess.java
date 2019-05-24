package protocol.commands.types.login;

import protocol.model.ChatMessage;
import protocol.commands.CommandType;
import protocol.commands.response.ResponseSuccess;

import java.util.List;

public class LoginResponseSuccess extends ResponseSuccess {
    private String sessionID;
    private List<ChatMessage> lastMessages;
    private List<String> users;

    public LoginResponseSuccess() {
        super(CommandType.COMMAND_LOGIN);
    }

    public String getSessionID() {
        return sessionID;
    }

    public void setSessionID(String sessionID) {
        this.sessionID = sessionID;
    }

    public List<ChatMessage> getLastMessages() {
        return lastMessages;
    }

    public void setLastMessages(List<ChatMessage> lastMessages) {
        this.lastMessages = lastMessages;
    }

    public List<String> getUsers() {
        return users;
    }

    public void setUsers(List<String> users) {
        this.users = users;
    }
}
