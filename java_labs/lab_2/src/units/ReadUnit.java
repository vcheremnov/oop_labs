package units;

import units.exceptions.ExecutionFailedException;

import java.io.*;

public class ReadUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (args.length == 0) {
            throw new IllegalArgumentException("ReadUnit error: input file name is expected");
        }

        String filename = args[0];
        try (BufferedReader inputFile = new BufferedReader(new FileReader(filename))) {
            return inputFile.lines().toArray(String[]::new);
        } catch (Exception ex) {
            throw new ExecutionFailedException("ReadUnit execution failed: " + ex.getMessage(), ex);
        }

    }
}
