package parser;

import java.io.*;

public class WordParser implements Parser {
    private boolean _hasReachedEOF = true;
    private BufferedReader _inputStream = null;

    private static int STRING_BUILDER_CAPACITY = 32;
    private StringBuilder word = new StringBuilder(STRING_BUILDER_CAPACITY);

    public WordParser(InputStream inputStream) {
        changeInputStream(inputStream);
    }

    /**
     * Parses words from the given input stream
     *
     * @return 1. Parsed word in case of successful parsing
     * 2. null if EOF has been reached whereas parsed word is empty
     * 3. null if called when the input stream has already reached EOF
     */
    @Override
    public String getNextToken() throws IOException {
        // clear the string builder
        word.setLength(0);

        // read characters from the stream, ignoring non-alphanumeric ones
        while (!hasReachedEOF()) {
            int c = _inputStream.read();
            if (c == -1) {
                _hasReachedEOF = true;
                break;
            } else if (Character.isLetterOrDigit(c)) {
                word.append((char) c);
            } else if (word.length() != 0) {
                break;
            }
        }

        return word.length() == 0 ? null : word.toString();
    }

    @Override
    public boolean hasReachedEOF() {
        return _hasReachedEOF;
    }

    @Override
    public void changeInputStream(InputStream inputStream) {
        if (inputStream == null) {
            throw new NullPointerException("Passed null input stream");
        }
        _inputStream = new BufferedReader(new InputStreamReader(inputStream));
        _hasReachedEOF = false;
    }
}
