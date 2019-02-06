package stats;

import parser.Parser;
import parser.WordParser;

import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class WordStatsCollector implements TextStatsCollector {
    private Map<String, Integer> _records = new HashMap<>();

    @Override
    public void collectData(InputStream is) {
        resetData();

        Parser parser = new WordParser(is);
        try {
            String word;
            while ((word = parser.getNextToken()) != null) {
                Integer counter = _records.getOrDefault(word, 0) + 1;
                _records.put(word, counter);
            }
        } catch (IOException ex) {
            System.err.println("Error: " + ex.getMessage());
        }
    }

    @Override
    public void printData(OutputStream os) {
        BufferedWriter outStream = new BufferedWriter(new OutputStreamWriter(os));
        try {
            for (String word: _records.keySet()) {
                outStream.write(word);
                outStream.newLine();
            }
            outStream.flush();
        } catch (IOException ex) {
            System.err.println("Error: " + ex.getMessage());
        }
    }

    @Override
    public void resetData() {
        _records.clear();
    }

    public void sortData() {

    }
}
