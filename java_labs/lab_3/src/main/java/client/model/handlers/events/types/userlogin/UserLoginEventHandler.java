package client.model.handlers.events.types.userlogin;

import client.model.session.ChatSession;
import client.model.handlers.MessageHandlerException;
import client.model.handlers.events.EventHandler;
import client.model.session.ChatSessionData;
import protocol.events.Event;
import protocol.events.types.userlogin.UserLoginEvent;

public class UserLoginEventHandler extends EventHandler {
    @Override
    protected void handleMessageImpl(Event message, ChatSession session) throws MessageHandlerException {
        UserLoginEvent event = (UserLoginEvent) message;
        String username = event.getUsername();

        ChatSessionData sessionData = session.getSessionData();
        sessionData.addUser(username);

//        session.notifyObservers();
        // TODO: notify observers!!!
    }

    @Override
    protected Class<? extends Event> getMessageClass() {
        return UserLoginEvent.class;
    }
}
