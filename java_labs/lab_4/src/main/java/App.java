import factory.CarManufacturing;
import factory.exceptions.FactoryLoadingException;

public class App {
    public static void main(String[] args) {
        JavafxApp.main(args);
    }
}

class JavafxApp {
    public static void main(String[] args) {
        CarManufacturing factory = null;
        try {
            factory = new CarManufacturing();
        } catch (FactoryLoadingException e) {
            e.printStackTrace();
            System.exit(-1);
        }
        factory.startProduction();
    }
}
