package protocol.commands.response;

import protocol.commands.CommandType;

public class ResponseError extends Response {
    public static final String REASON_FIELD_NAME = "reason";
    private String reason;

    public ResponseError(CommandType commandType) {
        super(commandType, ResponseStatus.RESPONSE_ERROR);
        reason = "";
    }

    public String getReason() {
        return reason;
    }

    public void setReason(String reason) {
        this.reason = reason;
    }
}
