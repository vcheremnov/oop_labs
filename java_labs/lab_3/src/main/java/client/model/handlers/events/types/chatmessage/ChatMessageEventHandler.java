package client.model.handlers.events.types.chatmessage;

import client.model.session.ChatSession;
import client.model.handlers.MessageHandlerException;
import client.model.handlers.events.EventHandler;
import client.model.session.ChatSessionData;
import protocol.events.Event;
import protocol.events.types.chatmessage.ChatMessageEvent;
import protocol.model.ChatMessage;

public class ChatMessageEventHandler extends EventHandler {
    @Override
    protected void handleMessageImpl(Event message, ChatSession session) throws MessageHandlerException {
        ChatMessageEvent event = (ChatMessageEvent) message;
        ChatMessage chatMessage = event.getChatMessage();

        ChatSessionData sessionData = session.getSessionData();
        sessionData.putMessage(chatMessage);

//        session.notifyObservers();
        // TODO: notify observers!!!
    }

    @Override
    protected Class<? extends Event> getMessageClass() {
        return ChatMessageEvent.class;
    }
}
