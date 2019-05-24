package server.handlers;

public class RequestHandlerException extends Exception {
    public RequestHandlerException() {
        super();
    }

    public RequestHandlerException(String message) {
        super(message);
    }

    public RequestHandlerException(String message, Throwable cause) {
        super(message, cause);
    }

    public RequestHandlerException(Throwable cause) {
        super(cause);
    }
}
