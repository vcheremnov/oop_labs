package server.handlers.types.chatmessage;

import protocol.commands.request.Request;
import protocol.commands.response.ResponseSuccess;
import protocol.commands.types.chatmessage.ChatMessageRequest;
import protocol.commands.types.chatmessage.ChatMessageResponseError;
import protocol.commands.types.chatmessage.ChatMessageResponseSuccess;
import protocol.events.types.chatmessage.ChatMessageEvent;
import protocol.model.ChatMessage;
import server.data.ChatDatabase;
import server.handlers.RequestHandler;
import misc.network.ClientSession;

public class ChatMessageRequestHandler extends RequestHandler {
    private final ChatDatabase database;

    public ChatMessageRequestHandler(ChatDatabase database) {
        super(database);
        this.database = database;
    }

    @Override
    protected void handleRequestImpl(Request request, ClientSession session) {
        boolean sessionValidated = verifySessionID(request, session, ChatMessageResponseError::new);
        if (!sessionValidated) {
            return;
        }

        ChatMessageRequest chatMessageRequest = (ChatMessageRequest) request;
        String sessionID = chatMessageRequest.getSessionID();
        String message = chatMessageRequest.getMessage();

        synchronized (database) {
            ChatMessage chatMessage = database.putMessage(sessionID, message);

            ResponseSuccess response = new ChatMessageResponseSuccess();
            setResponse(response);

            ChatMessageEvent event = new ChatMessageEvent();
            event.setChatMessage(chatMessage);
            setEvent(event);
        }
    }

    @Override
    protected Class<? extends Request> getRequestClass() {
        return ChatMessageRequest.class;
    }

}
