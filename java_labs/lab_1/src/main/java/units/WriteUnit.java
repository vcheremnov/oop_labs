package units;

import units.exceptions.UnitExecutionException;

import java.io.FileOutputStream;
import java.io.PrintWriter;

public class WriteUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) throws UnitExecutionException {
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
