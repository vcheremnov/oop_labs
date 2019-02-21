package factory.exceptions;

public class LoadingFailedException extends RuntimeException {
    public LoadingFailedException() {
        super();
    }

    public LoadingFailedException(String message) {
        super(message);
    }

    public LoadingFailedException(Throwable cause) {
        super(cause);
    }

    public LoadingFailedException(String message, Throwable cause) {
        super(message, cause);
    }
}
