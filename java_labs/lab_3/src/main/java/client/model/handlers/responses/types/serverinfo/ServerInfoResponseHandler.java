package client.model.handlers.responses.types.serverinfo;

import client.model.handlers.responses.ResponseHandler;
import client.model.session.ChatSession;
import protocol.Message;
import protocol.commands.types.serverinfo.ServerInfoResponseSuccess;
import protocol.model.ServerInfo;

public class ServerInfoResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) {
        ServerInfoResponseSuccess response = (ServerInfoResponseSuccess) message;
        ServerInfo serverInfo = response.getServerInfo();
        session.notifyObservers(ChatSession.Property.GOT_INFO, null, serverInfo);
    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return ServerInfoResponseSuccess.class;
    }
}
