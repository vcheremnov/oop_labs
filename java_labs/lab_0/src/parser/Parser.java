package parser;

import java.io.IOException;
import java.io.InputStream;

public interface Parser {
    String getNextToken() throws IOException;

    boolean hasReachedEOF();

    void changeInputStream(InputStream is);
}
