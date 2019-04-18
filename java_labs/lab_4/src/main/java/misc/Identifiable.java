package misc;

import java.util.concurrent.atomic.AtomicLong;

public class Identifiable {
    private static final AtomicLong nextID = new AtomicLong(0);
    private final String id;

    public Identifiable() {
        id = String.valueOf(nextID.getAndIncrement());
    }

    public String getID() {
        return id;
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

        Identifiable identifiable = (Identifiable) obj;
        return id.equals(identifiable.id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }
}
