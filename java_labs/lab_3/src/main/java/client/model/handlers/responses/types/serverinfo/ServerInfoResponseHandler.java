package client.model.handlers.responses.types.serverinfo;

import client.model.handlers.MessageHandlerException;
import client.model.handlers.responses.ResponseHandler;
import client.model.session.ChatSession;
import protocol.Message;
import protocol.commands.types.serverinfo.ServerInfoResponseSuccess;

public class ServerInfoResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) throws MessageHandlerException {

    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return ServerInfoResponseSuccess.class;
    }
}
