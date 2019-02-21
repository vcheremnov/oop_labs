package factory.exceptions;

public class UnitNotFoundException extends RuntimeException {
    public UnitNotFoundException() {
        super();
    }

    public UnitNotFoundException(String message) {
        super(message);
    }

    public UnitNotFoundException(Throwable cause) {
        super(cause);
    }

    public UnitNotFoundException(String message, Throwable cause) {
        super(message, cause);
    }
}
