package protocol.commands.types.logout;

import protocol.commands.CommandType;
import protocol.commands.request.Request;

public class LogoutRequest extends Request {
    public LogoutRequest() {
        super(CommandType.COMMAND_LOGOUT);
    }
}
