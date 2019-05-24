package protocol.commands;

public enum CommandType {
    COMMAND_LOGIN("login"),
    COMMAND_LOGOUT("logout"),
    COMMAND_SERVERINFO("serverinfo"),
    COMMAND_CHATMESSAGE("chatmessage");

    public static final String FIELD_NAME = "commandType";
    private final String commandType;

    CommandType(String commandType) {
        this.commandType = commandType;
    }

    @Override
    public String toString() {
        return commandType;
    }

}
