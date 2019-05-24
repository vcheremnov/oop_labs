package protocol.events.types.userlogin;

import protocol.events.Event;
import protocol.events.EventType;

public class UserLoginEvent extends Event {
    private String username;

    public UserLoginEvent() {
        super(EventType.EVENT_USERLOGIN);
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }
}
