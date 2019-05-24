package server.handlers.types.logout;

import protocol.commands.request.Request;
import protocol.commands.types.logout.LogoutRequest;
import protocol.commands.types.logout.LogoutResponseError;
import protocol.commands.types.logout.LogoutResponseSuccess;
import protocol.events.types.userlogout.UserLogoutEvent;
import server.handlers.RequestHandler;
import server.data.ChatDatabase;
import misc.network.ClientSession;

public class LogoutRequestHandler extends RequestHandler {
    private final ChatDatabase database;

    public LogoutRequestHandler(ChatDatabase database) {
        super(database);
        this.database = database;
    }

    @Override
    protected void handleRequestImpl(Request request, ClientSession session) {
        boolean sessionValidated = verifySessionID(request, session, LogoutResponseError::new);
        if (!sessionValidated) {
            return;
        }

        synchronized (database) {
            String sessionID = request.getSessionID();
            String username = database.removeClient(sessionID);
            setResponse(new LogoutResponseSuccess());

            UserLogoutEvent event = new UserLogoutEvent();
            event.setUsername(username);
            setEvent(event);

            session.setSessionID(null);
            session.setStartingEventNumber(Long.MAX_VALUE);
            setTerminateFlag();
        }
    }

    @Override
    protected Class<? extends Request> getRequestClass() {
        return LogoutRequest.class;
    }

}
