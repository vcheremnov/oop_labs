package units.exceptions;

public class UnitExecutionException extends Exception {
    public UnitExecutionException() {
        super();
    }

    public UnitExecutionException(String message) {
        super(message);
    }

    public UnitExecutionException(Throwable cause) {
        super(cause);
    }

    public UnitExecutionException(String message, Throwable cause) {
        super(message, cause);
    }
}
