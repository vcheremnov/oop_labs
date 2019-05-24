package protocol.commands.types.serverinfo;

import protocol.commands.CommandType;
import protocol.commands.request.Request;

public class ServerInfoRequest extends Request {
    public ServerInfoRequest() {
        super(CommandType.COMMAND_SERVERINFO);
    }
}
