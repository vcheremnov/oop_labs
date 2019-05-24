package server.handlers.types.serverinfo;

import protocol.commands.request.Request;
import protocol.commands.types.serverinfo.ServerInfoRequest;
import protocol.commands.types.serverinfo.ServerInfoResponseSuccess;
import server.data.ChatDatabase;
import server.handlers.RequestHandler;
import misc.network.ClientSession;

import java.util.List;

public class ServerInfoRequestHandler extends RequestHandler {
    private final ChatDatabase database;

    public ServerInfoRequestHandler(ChatDatabase database) {
        super(database);
        this.database = database;
    }

    @Override
    protected void handleRequestImpl(Request request, ClientSession session) {
        synchronized (database) {
            String serverName = database.getServerName();
            int maxClientsNumber = database.getMaxClientsNumber();
            List<String> users = database.getClientsNames();

            ServerInfoResponseSuccess response = new ServerInfoResponseSuccess();
            response.setServerName(serverName);
            response.setMaxClientsNumber(maxClientsNumber);
            response.setUsers(users);
            setResponse(response);
        }

        if (session.getSessionID() == null) {
            setTerminateFlag();
        }
    }

    @Override
    protected Class<? extends Request> getRequestClass() {
        return ServerInfoRequest.class;
    }
}
