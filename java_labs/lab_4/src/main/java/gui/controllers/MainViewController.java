package gui.controllers;

import factory.Factory;
import factory.Warehouse;
import factory.production.AssemblyShop;
import factory.sales.SalesDepartment;
import factory.supply.Supplies;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import misc.Delayable;

public class MainViewController {
    private Factory factory;

    @FXML
    private AnchorPane warehousesAP;

    @FXML
    private AnchorPane suppliesAP;

    @FXML
    private AnchorPane productionAP;

    @FXML
    private AnchorPane salesAP;

    @FXML
    private Button startButton;

    @FXML
    private Button stopButton;

    @FXML
    private Slider workersSlider;

    @FXML
    private Slider dealersSlider;

    @FXML
    private Slider bodySuppliersSlider;

    @FXML
    private Slider engineSuppliersSlider;

    @FXML
    private Slider accessorySuppliersSlider;

    @FXML
    private Label bodiesLabel;

    @FXML
    private Label enginesLabel;

    @FXML
    private Label accessoriesLabel;

    @FXML
    private Label carsLabel;

    @FXML
    private Label bodySuppliersLabel;

    @FXML
    private Label engineSuppliersLabel;

    @FXML
    private Label accessorySuppliersLabel;

    @FXML
    private Label bodiesProducedLabel;

    @FXML
    private Label enginesProducedLabel;

    @FXML
    private Label accessoriesProducedLabel;

    @FXML
    private Label workersNumberLabel;

    @FXML
    private Label producedNumberLabel;

    @FXML
    private Label requestedNumberLabel;

    @FXML
    private Label dealersNumberLabel;

    @FXML
    private Label soldNumberLabel;

    public void init(Stage stage, Factory factory) {
        this.factory = factory;

        stage.setOnCloseRequest(e -> {
            factory.stopProduction();
        });
    }

    private void initSliders() {
        workersSlider.setValue(factory.getAssemblyShop().getDelay());
        dealersSlider.setValue(factory.getSalesDepartment().getDelay());
        bodySuppliersSlider.setValue(factory.getBodySupplies().getDelay());
        engineSuppliersSlider.setValue(factory.getEngineSupplies().getDelay());
        accessorySuppliersSlider.setValue(factory.getAccessorySupplies().getDelay());
    }

    private void initTables() {
        setWarehouseInfo(bodiesLabel, factory.getBodyWarehouse());
        setWarehouseInfo(enginesLabel, factory.getEngineWarehouse());
        setWarehouseInfo(accessoriesLabel, factory.getAccessoryWarehouse());
        setWarehouseInfo(carsLabel, factory.getCarWarehouse());

        setSuppliesInfo(bodySuppliersLabel, bodiesProducedLabel, factory.getBodySupplies());
        setSuppliesInfo(engineSuppliersLabel, enginesProducedLabel, factory.getEngineSupplies());
        setSuppliesInfo(accessorySuppliersLabel, accessoriesProducedLabel, factory.getAccessorySupplies());

        setProductionInfo();
        setSalesInfo();
    }

    private void setWarehouseInfo(Label infoLabel, Warehouse<?> warehouse) {
        int itemsNumber = warehouse.getItemsNumber();
        int capacity = warehouse.getCapacity();
        infoLabel.setText(itemsNumber + "/" + capacity);
    }

    private void setSuppliesInfo(Label suppliersLabel, Label itemsProducedLabel, Supplies<?> supplies) {
        suppliersLabel.setText(String.valueOf(supplies.getSuppliersNumber()));
        itemsProducedLabel.setText(String.valueOf(supplies.getDetailsProduced()));
    }

    private void setProductionInfo() {
        AssemblyShop assemblyShop = factory.getAssemblyShop();
        workersNumberLabel.setText(String.valueOf(assemblyShop.getWorkersNumber()));
        producedNumberLabel.setText(String.valueOf(assemblyShop.getCarsProduced()));
        requestedNumberLabel.setText(String.valueOf(assemblyShop.getCarsRequested()));
    }

    private void setSalesInfo() {
        SalesDepartment salesDepartment = factory.getSalesDepartment();
        dealersNumberLabel.setText(String.valueOf(salesDepartment.getDealersNumber()));
        soldNumberLabel.setText(String.valueOf(salesDepartment.getCarsSold()));
    }

    private void setPropertiesListeners() {
        setSliderListener(bodySuppliersSlider, factory.getBodySupplies());
        setSliderListener(engineSuppliersSlider, factory.getEngineSupplies());
        setSliderListener(accessorySuppliersSlider, factory.getAccessorySupplies());
        setSliderListener(workersSlider, factory.getAssemblyShop());
        setSliderListener(dealersSlider, factory.getSalesDepartment());

        setWarehouseListener(factory.getBodyWarehouse(), bodiesLabel);
        setWarehouseListener(factory.getEngineWarehouse(), enginesLabel);
        setWarehouseListener(factory.getAccessoryWarehouse(), accessoriesLabel);
        setWarehouseListener(factory.getCarWarehouse(), carsLabel);

        setSuppliesListener(factory.getBodySupplies(), bodiesProducedLabel);
        setSuppliesListener(factory.getEngineSupplies(), enginesProducedLabel);
        setSuppliesListener(factory.getAccessorySupplies(), accessoriesProducedLabel);


        factory.getAssemblyShop().addPropertyChangeListener(AssemblyShop.Property.CARS_PRODUCED, evt -> {
            Platform.runLater(() -> {
                int carsProduced = (int) evt.getNewValue();
                producedNumberLabel.setText(String.valueOf(carsProduced));
            });
        });

        factory.getAssemblyShop().addPropertyChangeListener(AssemblyShop.Property.INCOMING_CARS, evt -> {
            Platform.runLater(() -> {
                int incomingCars = (int) evt.getNewValue();
                requestedNumberLabel.setText(String.valueOf(incomingCars));
            });
        });


        factory.getSalesDepartment().addPropertyChangeListener(SalesDepartment.Property.CARS_SOLD, evt -> {
            Platform.runLater(() -> {
                int carsSold = (int) evt.getNewValue();
                int dealersNumber = factory.getSalesDepartment().getDealersNumber();
                dealersNumberLabel.setText(String.valueOf(dealersNumber));
                soldNumberLabel.setText(String.valueOf(carsSold));
            });
        });
    }

    private void setWarehouseListener(Warehouse<?> warehouse, Label infoLabel) {
        warehouse.addPropertyChangeListener(Warehouse.Property.ITEMS_NUMBER, evt -> {
            Platform.runLater(() -> {
                int capacity = warehouse.getCapacity();
                int itemsNumber = (int) evt.getNewValue();
                infoLabel.setText(itemsNumber + "/" + capacity);
            });
        });
    }

    private void setSuppliesListener(Supplies<?> supplies, Label infoLabel) {
        supplies.addPropertyChangeListener(Supplies.Property.DETAILS_PRODUCED, evt -> {
            Platform.runLater(() -> {
                int detailsProduced = (int) evt.getNewValue();
                infoLabel.setText(String.valueOf(detailsProduced));
            });
        });
    }

    private void setSliderListener(Slider slider, Delayable delayable) {
        slider.valueProperty().addListener((observable, oldValue, newValue) -> {
            delayable.setDelay(newValue.intValue());
        });
    }

    @FXML
    private void startFactory(ActionEvent event) {
        factory.startProduction();
        initSliders();
        initTables();
        setPropertiesListeners();
        setActiveComponents(true);
    }

    @FXML
    private void stopFactory(ActionEvent event) {
        factory.stopProduction();
        setActiveComponents(false);
    }

    @FXML
    private void exitApp(ActionEvent event) {
        factory.stopProduction();
        Platform.exit();
    }

    private void setActiveComponents(boolean isStarted) {
        startButton.setDisable(isStarted);
        stopButton.setDisable(!isStarted);

        Node[] nodes = {
                workersSlider,
                dealersSlider,
                bodySuppliersSlider,
                engineSuppliersSlider,
                accessorySuppliersSlider,
                warehousesAP,
                suppliesAP,
                productionAP,
                salesAP
        };

        for (Node node: nodes) {
            node.setDisable(!isStarted);
        }
    }
}