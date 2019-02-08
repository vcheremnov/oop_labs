package parser;

import java.io.InputStream;

public abstract class StreamParser implements Parser {
    public abstract void setInputStream(InputStream inputStream);
}
