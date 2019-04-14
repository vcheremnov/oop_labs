package factory.production;

import factory.details.Accessory;
import factory.details.Body;
import factory.details.Engine;

import java.util.Objects;
import java.util.UUID;

public class Car {
    private UUID id = UUID.randomUUID();

    private Engine engine;
    private Body body;
    private Accessory accessory;

    public Car(Engine engine, Body body, Accessory accessory) {
        this.engine = Objects.requireNonNull(engine, "engine is null");
        this.body = Objects.requireNonNull(body, "car body is null");
        this.accessory = Objects.requireNonNull(accessory, "accessory is null");
    }

    public String getID() {
        return id.toString();
    }

    public String getEngineID() {
        return engine.getID();
    }

    public String getBodyID() {
        return body.getID();
    }

    public String getAccessoryID() {
        return accessory.getID();
    }
}
