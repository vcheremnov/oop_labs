package units;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import units.exceptions.UnitExecutionException;

import java.io.*;
import java.util.Arrays;

public class ReadUnit implements Unit {
    private static final Logger logger = LogManager.getLogger(ReadUnit.class);
    @Override
    public String[] execute(String[] input, String... args) throws UnitExecutionException {
        logger.info("Executing ReadUnit with args " + Arrays.asList(args));
        if (args.length != 1) {
            throw new IllegalArgumentException("ReadUnit error: input file name is expected");
        }

        String filename = args[0];
        try (BufferedReader inputFile = new BufferedReader(new FileReader(filename))) {
            return inputFile.lines().toArray(String[]::new);
        } catch (Exception ex) {
            throw new UnitExecutionException("ReadUnit execution failed: " + ex.getMessage(), ex);
        }

    }
}
