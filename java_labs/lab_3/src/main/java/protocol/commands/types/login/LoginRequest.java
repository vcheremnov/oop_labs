package protocol.commands.types.login;

import protocol.commands.request.Request;
import protocol.commands.CommandType;

public class LoginRequest extends Request {
    private String username;

    public LoginRequest() {
        super(CommandType.COMMAND_LOGIN);
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }
}
