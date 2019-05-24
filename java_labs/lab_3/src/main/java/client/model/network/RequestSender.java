package client.model.network;

import com.google.gson.Gson;
import misc.network.ClientSession;
import protocol.commands.request.Request;

import java.io.IOException;

public class RequestSender {
    private static final Gson gson = new Gson();

    private ClientSession session;

    public RequestSender(ClientSession session) {
        this.session = session;
    }

    public void sendRequest(Request request) throws IOException {
        String requestAsJson = gson.toJson(request);
        session.writeMessage(requestAsJson);
    }
}
