package client.model.handlers;

public class MessageHandlerException extends Exception {
    public MessageHandlerException() {
        super();
    }

    public MessageHandlerException(String message) {
        super(message);
    }

    public MessageHandlerException(String message, Throwable cause) {
        super(message, cause);
    }

    public MessageHandlerException(Throwable cause) {
        super(cause);
    }
}
