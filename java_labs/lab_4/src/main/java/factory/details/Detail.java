package factory.details;

import java.util.UUID;

public abstract class Detail {
    private UUID id = UUID.randomUUID();

    public String getID() {
        return id.toString();
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (this.getClass() != obj.getClass()) {
            return false;
        }

        Detail detail = (Detail) obj;
        return id.equals(detail.id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }
}
