package gui;

import factory.Factory;
import factory.exceptions.FactoryLoadingException;
import gui.controllers.MainViewController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.net.URL;

public class JavafxApp extends Application {
    private static Factory factory;

    public static void main(String[] args) {
        try {
            factory = new Factory();
        } catch (FactoryLoadingException e) {
            System.err.println("Failed to create a car factory");
            e.printStackTrace();
            System.exit(-1);
        }
        Application.launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader();
        URL guiLocation = getClass().getClassLoader().getResource("gui.fxml");
        loader.setLocation(guiLocation);

        Parent root = loader.load();
        Scene scene = new Scene(root);

        MainViewController controller = loader.getController();
        controller.init(primaryStage, factory);

        primaryStage.setTitle("Car factory");
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
