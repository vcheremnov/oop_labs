package protocol.model;

public class ChatMessage implements Comparable<ChatMessage> {
    private long date;
    private String senderName;
    private String message;

    public long getDate() {
        return date;
    }

    public void setDate(long date) {
        this.date = date;
    }

    public String getSenderName() {
        return senderName;
    }

    public void setSenderName(String senderName) {
        this.senderName = senderName;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    @Override
    public int compareTo(ChatMessage chatMessage) {
        int dateCompareResult = Long.compare(date, chatMessage.date);
        if (dateCompareResult != 0) {
            return dateCompareResult;
        }

        int senderCompareResult = senderName.compareTo(chatMessage.senderName);
        if (senderCompareResult != 0) {
            return senderCompareResult;
        }

        return message.compareTo(chatMessage.message);
    }
}
