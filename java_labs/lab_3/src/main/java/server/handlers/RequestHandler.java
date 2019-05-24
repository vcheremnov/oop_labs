package server.handlers;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import protocol.commands.request.Request;
import protocol.commands.response.Response;
import protocol.commands.response.ResponseError;
import protocol.events.Event;
import server.data.ChatDatabase;
import misc.network.ClientSession;

import java.util.function.Supplier;


public abstract class RequestHandler {
    private static final Gson gson = new Gson();

    private final ChatDatabase database;

    private boolean terminateFlag = false;
    private Response response;
    private Event event;

    public RequestHandler(ChatDatabase database) {
        this.database = database;
    }

    public final void handleRequest(String request, ClientSession session) throws RequestHandlerException {
        try {
            Request requestObject = gson.fromJson(request, getRequestClass());
            if (requestObject == null) {
                setTerminateFlag();
                throw new RequestHandlerException("Empty request body");
            }

            handleRequestImpl(requestObject, session);
        } catch (JsonSyntaxException ex) {
            setTerminateFlag();
            throw new RequestHandlerException("Failed to parse request", ex);
        }
    }

    public String getResponse() {
        return (response == null) ? null : gson.toJson(response);
    }

    public String getEvent() {
        return (event == null) ? null : gson.toJson(event);
    }

    public long getEventSerialNumber() {
        return (event == null) ? -1 : event.getSerialNumber();
    }

    public boolean isTerminateFlagSet() {
        return terminateFlag;
    }

    protected void setTerminateFlag() {
        terminateFlag = true;
    }

    protected void setResponse(Response response) {
        this.response = response;
    }

    protected void setEvent(Event event) {
        this.event = event;
    }

    protected boolean verifySessionID(Request request, ClientSession session,
                                      Supplier<? extends ResponseError> errorResponseCtor)  {
        String requestSessionID = request.getSessionID();
        String realSessionID = session.getSessionID();
        ResponseError responseError = errorResponseCtor.get();

        if (requestSessionID == null) {
            responseError.setReason("Session ID specified in the request is null");
        } else if (!requestSessionID.equals(realSessionID)) {
            responseError.setReason("Session ID specified in the request doesn't match " +
                                    "to the assigned session ID");
        } else if (!database.sessionExists(requestSessionID)) {
            responseError.setReason("Session ID specified in the request does not exist");
        } else {
            return true;
        }

        setResponse(responseError);
        setTerminateFlag();

        return false;
    }

    protected abstract void handleRequestImpl(Request request, ClientSession session) throws RequestHandlerException;

    protected abstract Class<? extends Request> getRequestClass();
}