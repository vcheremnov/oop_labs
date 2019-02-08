package datawriter;

import java.io.OutputStream;

public abstract class StreamDataWriter implements DataWriter {
    public abstract void setOutputStream(OutputStream outputStream);
}
