package protocol.commands.response;

import protocol.commands.CommandType;

public class ResponseSuccess extends Response {
    public ResponseSuccess(CommandType commandType) {
        super(commandType, ResponseStatus.RESPONSE_SUCCESS);
    }
}
