package protocol.commands.types.serverinfo;

import protocol.commands.CommandType;
import protocol.commands.response.ResponseSuccess;
import protocol.model.ServerInfo;

import java.util.List;

public class ServerInfoResponseSuccess extends ResponseSuccess {
    private ServerInfo serverInfo;

    public ServerInfoResponseSuccess() {
        super(CommandType.COMMAND_SERVERINFO);
    }

    public void setServerInfo(ServerInfo serverInfo) {
        this.serverInfo = serverInfo;
    }

    public ServerInfo getServerInfo() {
        return serverInfo;
    }
}
