package server.handlers.types.login;

import protocol.model.ChatMessage;
import protocol.commands.request.Request;
import protocol.commands.response.ResponseError;
import protocol.events.types.userlogin.UserLoginEvent;
import server.data.ChatDatabase;
import protocol.commands.types.login.LoginRequest;
import protocol.commands.types.login.LoginResponseError;
import protocol.commands.types.login.LoginResponseSuccess;
import server.handlers.RequestHandler;
import misc.network.ClientSession;

import java.util.List;

public class LoginRequestHandler extends RequestHandler {

    private final ChatDatabase database;

    public LoginRequestHandler(ChatDatabase database) {
        super(database);
        this.database = database;
    }

    @Override
    public void handleRequestImpl(Request request, ClientSession session) {
        if (session.getSessionID() != null) {
            setLoginResponseError("Login request was received whereas session has been established");
            setTerminateFlag();
            return;
        }

        LoginRequest loginRequest = (LoginRequest) request;
        String username = loginRequest.getUsername();

        synchronized (database) {
            if (database.chatRoomIsFull()) {
                setLoginResponseError("Chat room is full");
                setTerminateFlag();
                return;
            }

            if (database.clientExists(username)) {
                String errorReason = String.format("User \"%s\" already exists", username);
                setLoginResponseError(errorReason);
                setTerminateFlag();
                return;
            }

            String sessionID = database.addClient(username);
            List<ChatMessage> lastMessages = database.getLastMessages();
            List<String> clientsNames = database.getClientsNames();

            LoginResponseSuccess response = new LoginResponseSuccess();
            response.setSessionID(sessionID);
            response.setLastMessages(lastMessages);
            response.setUsers(clientsNames);
            setResponse(response);

            UserLoginEvent event = new UserLoginEvent();
            event.setUsername(username);
            setEvent(event);

            long eventSerialNumber = event.getSerialNumber();
            session.setSessionID(sessionID);
            session.setStartingEventNumber(eventSerialNumber);
        }

    }

    private void setLoginResponseError(String errorReason) {
        ResponseError response = new LoginResponseError();
        response.setReason(errorReason);
        setResponse(response);
    }

    @Override
    protected Class<? extends Request> getRequestClass() {
        return LoginRequest.class;
    }

}
