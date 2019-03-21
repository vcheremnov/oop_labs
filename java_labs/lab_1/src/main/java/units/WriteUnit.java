package units;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import units.exceptions.UnitExecutionException;

import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.Arrays;

public class WriteUnit implements Unit {
    private static final Logger logger = LogManager.getLogger(WriteUnit.class);
    @Override
    public String[] execute(String[] input, String... args) throws UnitExecutionException {
        logger.info("Executing WriteUnit with args " + Arrays.asList(args));
        if (input == null) {
            throw new IllegalArgumentException("WriteUnit error: input is null");
        }
        if (args.length != 1) {
            throw new IllegalArgumentException("WriteUnit error: output file name is expected");
        }

        String filename = args[0];
        try (PrintWriter outputFile = new PrintWriter(new FileOutputStream(filename))) {
            for (String line: input) {
                outputFile.println(line);
            }
        } catch (Exception ex) {
            throw new UnitExecutionException("WriteUnit execution failure: " + ex.getMessage(), ex);
        }
        // return input for further processing
        return input;
    }
}
