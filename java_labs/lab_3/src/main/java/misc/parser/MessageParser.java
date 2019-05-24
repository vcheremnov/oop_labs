package misc.parser;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.google.gson.JsonSyntaxException;
import protocol.MessageType;
import protocol.commands.CommandType;
import protocol.commands.response.ResponseError;
import protocol.commands.response.ResponseStatus;
import protocol.events.EventType;


public class MessageParser {
    private final JsonParser jsonParser = new JsonParser();
    private final RequestParser requestParser = new RequestParser();
    private final ResponseParser responseParser = new ResponseParser();
    private final EventParser eventParser = new EventParser();

    private String messageType;
    private JsonObject jsonObject = new JsonObject();

    public void parseMessage(String message) throws HandlerTypeParserException {
        jsonObject = makeJsonObject(message);
        messageType = parseMessageType();

        if (messageType.equals(MessageType.MESSAGE_REQUEST.toString())) {
            requestParser.parseRequest();
        } else if (messageType.equals(MessageType.MESSAGE_RESPONSE.toString())) {
            responseParser.parseResponse();
        } else if (messageType.equals(MessageType.MESSAGE_EVENT.toString())) {
            eventParser.parseEvent();
        }
    }

    public String getMessageType() {
        return messageType;
    }

    private JsonObject makeJsonObject(String message) throws HandlerTypeParserException {
        try {
            JsonElement jsonElement = jsonParser.parse(message);
            if (!jsonElement.isJsonObject()) {
                throw new HandlerTypeParserException("Not a json object");
            }
            return jsonElement.getAsJsonObject();
        } catch (JsonSyntaxException ex) {
            throw new HandlerTypeParserException("Failed to parse response", ex);
        }
    }

    private String parseMessageType() throws HandlerTypeParserException {
        JsonElement messageTypeField = jsonObject.get(MessageType.FIELD_NAME);
        if (messageTypeField.isJsonNull()) {
            throw new HandlerTypeParserException("Not a message");
        }

        return messageTypeField.getAsString();
    }

    private String getFieldValue(String fieldName, String errorMessage) throws HandlerTypeParserException {
        JsonElement field = jsonObject.get(fieldName);
        if (field == null) {
            throw new HandlerTypeParserException(errorMessage);
        }
        return field.getAsString();
    }


    public class RequestParser {
        private String requestType;

        RequestParser() {}

        public String getRequestType() {
            return requestType;
        }

        void parseRequest() throws HandlerTypeParserException {
            String errorMessage = "Request of unspecified type";
            requestType = getFieldValue(CommandType.FIELD_NAME, errorMessage);
        }
    }


    public class ResponseParser {
        private String responseType;
        private String responseStatus;
        private String errorReason;

        ResponseParser() {}

        public String getResponseType() {
            return responseType;
        }

        public String getResponseStatus() {
            return responseStatus;
        }

        public String getErrorReason() {
            return errorReason;
        }

        void parseResponse() throws HandlerTypeParserException {
            String errorMessage = "Response of unspecified type";
            responseType = getFieldValue(CommandType.FIELD_NAME, errorMessage);

            errorMessage = "Response with unspecified status";
            responseStatus = getFieldValue(ResponseStatus.FIELD_NAME, errorMessage);

            if (responseStatus.equals(ResponseStatus.RESPONSE_ERROR.toString())) {
                errorMessage = "Reason of error response is not specified";
                errorReason = getFieldValue(ResponseError.REASON_FIELD_NAME, errorMessage);
            } else {
                errorReason = null;
            }
        }
    }

    public class EventParser {
        private String eventType;

        EventParser() {}

        public String getEventType() {
            return eventType;
        }

        void parseEvent() throws HandlerTypeParserException {
            String errorMessage = "Event of unspecified type";
            eventType = getFieldValue(EventType.FIELD_NAME, errorMessage);
        }
    }

    public RequestParser getRequestParser() {
        return requestParser;
    }

    public ResponseParser getResponseParser() {
        return responseParser;
    }

    public EventParser getEventParser() {
        return eventParser;
    }
}
