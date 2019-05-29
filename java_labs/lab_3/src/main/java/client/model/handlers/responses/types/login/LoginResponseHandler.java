package client.model.handlers.responses.types.login;

import client.model.handlers.responses.ResponseHandler;
import client.model.session.ChatSession;
import client.model.session.ChatSessionData;
import protocol.Message;
import protocol.commands.types.login.LoginResponseSuccess;
import protocol.model.ChatMessage;

import java.util.List;

public class LoginResponseHandler extends ResponseHandler {
    @Override
    protected void handleMessageImpl(Message message, ChatSession session) {
        LoginResponseSuccess response = (LoginResponseSuccess) message;
        String sessionID = response.getSessionID();
        List<ChatMessage> lastMessages = response.getLastMessages();
        List<String> users = response.getUsers();

        ChatSessionData sessionData = session.getSessionData();
        sessionData.addUsers(users);
        sessionData.setSessionID(sessionID);
        for (ChatMessage chatMessage: lastMessages) {
            sessionData.putMessage(chatMessage);
        }

        session.notifyObservers(ChatSession.Property.LOGGED_IN, null, true);
    }

    @Override
    protected Class<? extends Message> getMessageClass() {
        return LoginResponseSuccess.class;
    }
}
