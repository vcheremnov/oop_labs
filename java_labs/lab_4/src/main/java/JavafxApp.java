import factory.Factory;
import factory.exceptions.FactoryLoadingException;
import javafx.application.Application;
import javafx.stage.Stage;

public class JavafxApp extends Application {
    private static Factory factory;

    public static void main(String[] args) {
        try {
            factory = new Factory();
        } catch (FactoryLoadingException e) {
            System.err.println("Failed to load the car manufacturing");
            e.printStackTrace();
            System.exit(-1);
        }
        Application.launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
//        Pane startPane = new HBox(5);
//
//        ToggleButton button = new ToggleButton("Start");
//        Label state = new Label("");
//        button.setOnAction((event -> {
//        }));
//        startPane.getChildren().addAll(button, state);
//
//        Pane sliderPane = new HBox(5);
//
//        Slider dealerDelaySlider = new Slider(0, 1000, manufacturing.getDealers().get(0).getDelay());
//        dealerDelaySlider.valueProperty().addListener((observable, oldValue, newValue) -> {
//            for (Dealer dealer: manufacturing.getDealers()) {
//                dealer.setDelay(newValue.intValue());
//            }
//        });
//        dealerDelaySlider.setOrientation(Orientation.VERTICAL);
//        dealerDelaySlider.setShowTickMarks(true);
//        dealerDelaySlider.setShowTickLabels(true);
//        dealerDelaySlider.setBlockIncrement(100);
//        dealerDelaySlider.setMajorTickUnit(200);
//        dealerDelaySlider.setMinorTickCount(10);
//
//
//        Slider workerDelaySlider = new Slider(100, 2000, manufacturing.getAssemblyShop().getDelay());
//        workerDelaySlider.valueProperty().addListener((observable, oldValue, newValue) -> {
//            manufacturing.getAssemblyShop().setDelay(newValue.intValue());
//        });
//
//        Slider bodyProviderDelaySlider = new Slider(100, 2000, manufacturing.getBodyProvider().getDelay());
//        bodyProviderDelaySlider.valueProperty().addListener((observable, oldValue, newValue) -> {
//            manufacturing.getBodyProvider().setDelay(newValue.intValue());
//        });
//
//        Slider engineProviderDelaySlider = new Slider(100, 2000, manufacturing.getEngineProvider().getDelay());
//        engineProviderDelaySlider.valueProperty().addListener((observable, oldValue, newValue) -> {
//            manufacturing.getEngineProvider().setDelay(newValue.intValue());
//        });
//
//        Slider accessoryDelaySlider = new Slider(100, 2000, manufacturing.getAccessoryProviders().get(0).getDelay());
//        accessoryDelaySlider.valueProperty().addListener((observable, oldValue, newValue) -> {
//            for (Provider<?> provider: manufacturing.getAccessoryProviders()) {
//                provider.setDelay(newValue.intValue());
//            }
//        });
//
//        sliderPane.getChildren().addAll(engineProviderDelaySlider, dealerDelaySlider, accessoryDelaySlider,
//                                        workerDelaySlider, bodyProviderDelaySlider);
//
//        Pane root = new VBox(10, startPane, sliderPane);
//
//        Scene scene = new Scene(root, 300, 400, Color.ANTIQUEWHITE);
//        primaryStage.setScene(scene);
//        primaryStage.setTitle("hello");
//        primaryStage.setOnCloseRequest(event -> manufacturing.stopProduction());
//
//        primaryStage.show();
    }
}
