package protocol.commands.types.logout;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseError;

public class LogoutResponseError extends ResponseError {
    public LogoutResponseError() {
        super(CommandType.COMMAND_LOGOUT);
    }
}
