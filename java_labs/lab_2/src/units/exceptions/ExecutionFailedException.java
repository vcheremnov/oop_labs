package units.exceptions;

public class ExecutionFailedException extends RuntimeException {
    public ExecutionFailedException() {
        super();
    }

    public ExecutionFailedException(String message) {
        super(message);
    }

    public ExecutionFailedException(Throwable cause) {
        super(cause);
    }

    public ExecutionFailedException(String message, Throwable cause) {
        super(message, cause);
    }
}
