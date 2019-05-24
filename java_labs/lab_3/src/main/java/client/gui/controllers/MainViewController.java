package client.gui.controllers;

import client.gui.JavafxApp;
import client.model.session.ChatSession;
import client.model.session.ChatSessionData;
import javafx.application.Platform;
import javafx.beans.InvalidationListener;
import javafx.beans.property.Property;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;

public class MainViewController {
    public void init(Stage stage) {
        portTextField.textProperty().addListener((observable, oldValue, newValue) -> {
            if (!newValue.matches("\\d*")) {
                portTextField.setText(newValue.replaceAll("[^\\d]", ""));
            }
        });
    }

    @FXML
    private Label responseLabel;

    @FXML
    private TextField portTextField;

    @FXML
    private TextField usernameTextField;

    @FXML
    private Button connectButton;

    @FXML
    private TextField addressTextField;

    @FXML
    void connect(ActionEvent event) {
        ChatSession chatSession = new ChatSession();
        String username = usernameTextField.getText();
        String address = addressTextField.getText();
        int port = Integer.parseInt(portTextField.getText());
        try {
            // TODO: make a background task
            chatSession.connect(address, port);
            final ChatSessionData sessionData = chatSession.getSessionData();
            chatSession.addPropertyChangeListener(ChatSession.Property.LOGGED_IN_PROPERTY, evt -> {
                boolean hasLoggedIn = (boolean) evt.getNewValue();
                if (hasLoggedIn) {
                    Platform.runLater(() -> {
                        // open new window
                        responseLabel.setText("LOGGED IN");
                    });
                }
            });
            chatSession.startSession(username, (errorMessage) -> Platform.runLater(() -> {
                responseLabel.setText(errorMessage);
            }));
        } catch (IOException e) {
            responseLabel.setText(e.getMessage());
            chatSession.disconnect();
            e.printStackTrace();
        }
    }
}
