package client.gui;

import client.gui.controllers.MainViewController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.net.URL;

public class JavafxApp extends Application {
    public static void main(String[] args) {
        Application.launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader();
        URL guiLocation = getClass().getClassLoader().getResource("client/gui/mainview.fxml");
        loader.setLocation(guiLocation);

        Parent root = loader.load();
        Scene scene = new Scene(root);

        MainViewController controller = loader.getController();
        controller.init(primaryStage);

        primaryStage.setTitle("Chat client");
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
