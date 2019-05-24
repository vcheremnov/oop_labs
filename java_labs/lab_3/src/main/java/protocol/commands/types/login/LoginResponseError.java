package protocol.commands.types.login;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseError;

public class LoginResponseError extends ResponseError {
    public LoginResponseError() {
        super(CommandType.COMMAND_LOGIN);
    }
}
