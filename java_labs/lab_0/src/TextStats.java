import datawriter.CSVWriter;
import parser.WordParser;
import stats.WordStatsCollector;

import java.io.*;


final public class TextStats {
    private static WordStatsCollector _statsCollector = new WordStatsCollector();

    public static void main(String[] args) {
        if (args.length == 0) {
            // interactive mode
            _statsCollector.collectWords(new WordParser(System.in));
            _statsCollector.printWordStats(new CSVWriter(System.out));
        } else if (args.length == 2) {
            // file mode
            try (FileInputStream inputFile = new FileInputStream(args[0]);
                 FileOutputStream outputFile = new FileOutputStream(args[1])) {
                _statsCollector.collectWords(new WordParser(inputFile));
                _statsCollector.printWordStats(new CSVWriter(outputFile));
            } catch (IOException ex) {
                System.out.println("Error: " + ex.getMessage());
            }
        } else {
            System.err.println("Usage: <input file> <output file>");
        }
    }
}
