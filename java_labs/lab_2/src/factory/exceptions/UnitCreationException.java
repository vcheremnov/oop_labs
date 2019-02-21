package factory.exceptions;

public class UnitCreationException extends RuntimeException {
    public UnitCreationException() {
        super();
    }

    public UnitCreationException(String message) {
        super(message);
    }

    public UnitCreationException(Throwable cause) {
        super(cause);
    }

    public UnitCreationException(String message, Throwable cause) {
        super(message, cause);
    }
}
