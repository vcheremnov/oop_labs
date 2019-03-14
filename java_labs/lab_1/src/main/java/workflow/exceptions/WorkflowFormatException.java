package workflow.exceptions;

public class WorkflowFormatException extends Exception {
    public WorkflowFormatException() {
        super();
    }

    public WorkflowFormatException(String message) {
        super(message);
    }

    public WorkflowFormatException(Throwable cause) {
        super(cause);
    }

    public WorkflowFormatException(String message, Throwable cause) {
        super(message, cause);
    }
}
