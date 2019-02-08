package parser;

import java.io.*;

public class WordParser extends StreamParser {
    private BufferedReader _inputStream = null;
    private StringBuilder word = new StringBuilder();

    public WordParser(InputStream inputStream) {
        setInputStream(inputStream);
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
        for (int c; (c = _inputStream.read()) != -1; ) {
            if (Character.isLetterOrDigit(c)) {
                word.append((char) c);
            } else if (word.length() != 0) {
                break;
            }
        }

        return word.length() == 0 ? null : word.toString();
    }

    @Override
    public void setInputStream(InputStream inputStream) {
        if (inputStream == null) {
            throw new NullPointerException("Passed input stream is null");
        }
        _inputStream = new BufferedReader(new InputStreamReader(inputStream));
    }
}
