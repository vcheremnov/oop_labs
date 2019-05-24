package client.model.handlers.events.types.userlogout;

import client.model.session.ChatSession;
import client.model.handlers.MessageHandlerException;
import client.model.handlers.events.EventHandler;
import client.model.session.ChatSessionData;
import protocol.events.Event;
import protocol.events.types.userlogout.UserLogoutEvent;

public class UserLogoutEventHandler extends EventHandler {
    @Override
    protected void handleMessageImpl(Event message, ChatSession session) throws MessageHandlerException {
        UserLogoutEvent event = (UserLogoutEvent) message;
        String username = event.getUsername();

        ChatSessionData sessionData = session.getSessionData();
        sessionData.removeUser(username);

//        session.notifyObservers();
        // TODO: notify observers!!!
    }

    @Override
    protected Class<? extends Event> getMessageClass() {
        return UserLogoutEvent.class;
    }
}
