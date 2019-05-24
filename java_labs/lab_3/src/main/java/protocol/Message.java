package protocol;

public class Message {
    private String messageType;

    public Message(MessageType messageType) {
        this.messageType = messageType.toString();
    }

    public String getMessageType() {
        return messageType;
    }
}
