package protocol.events;

public enum EventType {
    EVENT_CHATMESSAGE("chatmessage"),
    EVENT_USERLOGIN("userlogin"),
    EVENT_USERLOGOUT("userlogout"),
    EVENT_NOPLACES("noplaces");

    public static final String FIELD_NAME = "eventType";
    private final String eventType;

    EventType(String eventType) {
        this.eventType = eventType;
    }

    @Override
    public String toString() {
        return eventType;
    }
}
