import stats.TextStatsCollector;
import stats.WordStatsCollector;

import java.io.*;


final public class TextStats {
    private static TextStatsCollector _statsCollector = new WordStatsCollector();

    public static void main(String[] args) {
        WordStatsCollector stats = new WordStatsCollector();
        if (args.length == 0) {
            // interactive mode
            _statsCollector.collectData(System.in);
            _statsCollector.printData(System.out);
        } else if (args.length != 2) {
            System.err.println("Usage: <input file> <output file>");
        } else {
            // file mode
            try (FileInputStream inputFile = new FileInputStream(args[0]);
                 FileOutputStream outputFile = new FileOutputStream(args[1])) {
                _statsCollector.collectData(inputFile);
                _statsCollector.printData(outputFile);
            } catch (IOException ex) {
                System.out.println("Error: " + ex.getMessage());
            }
        }
    }
}
