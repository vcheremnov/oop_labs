package protocol.events.types.userlogout;

import protocol.events.Event;
import protocol.events.EventType;

public class UserLogoutEvent extends Event {
    private String username;

    public UserLogoutEvent() {
        super(EventType.EVENT_USERLOGOUT);
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }
}
