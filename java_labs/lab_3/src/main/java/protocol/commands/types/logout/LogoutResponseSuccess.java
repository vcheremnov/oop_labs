package protocol.commands.types.logout;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseSuccess;

public class LogoutResponseSuccess extends ResponseSuccess {
    public LogoutResponseSuccess() {
        super(CommandType.COMMAND_LOGOUT);
    }
}
