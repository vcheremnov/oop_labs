package server.network;

import com.google.gson.Gson;
import protocol.events.Event;
import protocol.events.types.userlogout.UserLogoutEvent;
import server.data.ChatDatabase;

class DisconnectHandler {
    private static final Gson gson = new Gson();
    private ChatDatabase database;
    private Event event;

    DisconnectHandler(ChatDatabase database) {
        this.database = database;
    }

    void handleDisconnect(String sessionID) {
        String clientName = database.removeClient(sessionID);
        if (clientName == null) {
            return;
        }

        UserLogoutEvent userLogoutEvent = new UserLogoutEvent();
        userLogoutEvent.setUsername(clientName);
        event = userLogoutEvent;
    }

    String getEvent() {
        return (event == null) ? null : gson.toJson(event);
    }

    long getEventSerialNumber() {
        return (event == null) ? -1 : event.getSerialNumber();
    }
}
