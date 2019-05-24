package client.model.handlers.responses.types.logout;

import client.model.session.ChatSession;
import client.model.handlers.MessageHandlerException;
import client.model.handlers.responses.ResponseHandler;
import protocol.Message;
import protocol.commands.types.logout.LogoutResponseSuccess;

public class LogoutResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) throws MessageHandlerException {
        LogoutResponseSuccess response = (LogoutResponseSuccess) message;
//        session.notifyObservers();
        // TODO: notify observers!!!
    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return LogoutResponseSuccess.class;
    }
}
