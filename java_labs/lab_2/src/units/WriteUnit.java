package units;

import units.exceptions.ExecutionFailedException;

import java.io.FileOutputStream;
import java.io.PrintWriter;

public class WriteUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (input == null) {
            throw new IllegalArgumentException("WriteUnit error: input is null");
        }
        if (args.length == 0) {
            throw new IllegalArgumentException("WriteUnit error: output file name is expected");
        }

        try (PrintWriter outputFile = new PrintWriter(new FileOutputStream(args[0]))) {
            for (String line: input) {
                outputFile.println(line);
            }
        } catch (Exception ex) {
            throw new ExecutionFailedException("Execution failure", ex);
        }

        return null;
    }
}
