package datawriter;

import java.io.OutputStream;
import java.io.PrintWriter;

public class CSVWriter extends StreamDataWriter {
    private PrintWriter _outStream = null;
    private int _columnsPerLine = -1;

    public CSVWriter(OutputStream outputStream) {
        setOutputStream(outputStream);
    }

    @Override
    public void setOutputStream(OutputStream outputStream) {
        _outStream = new PrintWriter(outputStream, true);
    }

    @Override
    public void write(String[] csvColumns) {
        if (_columnsPerLine == -1) {
            _columnsPerLine = csvColumns.length;
        } else if (_columnsPerLine != csvColumns.length) {
            throw new IllegalArgumentException("Invalid number of columns: " +
                           String.format("got %d, expected %d", csvColumns.length, _columnsPerLine));
        }
        _outStream.println(String.join(",", csvColumns));
    }
}
