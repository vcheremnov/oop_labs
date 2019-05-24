package protocol.events.types.chatmessage;

import protocol.model.ChatMessage;
import protocol.events.Event;
import protocol.events.EventType;

public class ChatMessageEvent extends Event {
    private ChatMessage chatMessage;

    public ChatMessageEvent() {
        super(EventType.EVENT_CHATMESSAGE);
    }

    public ChatMessage getChatMessage() {
        return chatMessage;
    }

    public void setChatMessage(ChatMessage chatMessage) {
        this.chatMessage = chatMessage;
    }
}
