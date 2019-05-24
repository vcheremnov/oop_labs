package protocol.commands.response;

public enum ResponseStatus {
    RESPONSE_ERROR("error"),
    RESPONSE_SUCCESS("success");

    public static final String FIELD_NAME = "status";
    private final String responseStatus;

    ResponseStatus(String responseStatus) {
        this.responseStatus = responseStatus;
    }

    @Override
    public String toString() {
        return responseStatus;
    }
}
