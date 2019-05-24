package protocol;

public enum MessageType {
    MESSAGE_REQUEST("request"),
    MESSAGE_RESPONSE("response"),
    MESSAGE_EVENT("event");

    public static final String FIELD_NAME = "messageType";
    private final String messageType;

    MessageType(String messageType) {
        this.messageType = messageType;
    }

    @Override
    public String toString() {
        return messageType;
    }

}
