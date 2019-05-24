package protocol.commands.types.serverinfo;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseError;

public class ServerInfoResponseError extends ResponseError {
    public ServerInfoResponseError() {
        super(CommandType.COMMAND_SERVERINFO);
    }
}
