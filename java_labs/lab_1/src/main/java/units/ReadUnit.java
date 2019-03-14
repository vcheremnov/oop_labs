package units;

import units.exceptions.UnitExecutionException;

import java.io.*;

public class ReadUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) throws UnitExecutionException {
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
