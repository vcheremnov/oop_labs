package client.model.network;

@FunctionalInterface
public interface ErrorAction {
    ErrorAction NO_ACTION = null;

    void handleError(String errorMessage);
}
