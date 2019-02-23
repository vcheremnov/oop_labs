package units;

import units.exceptions.ExecutionFailedException;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class ReadUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (args.length == 0) {
            throw new IllegalArgumentException("ReadUnit error: input file name is expected");
        }

        try (BufferedReader inputFile = new BufferedReader(new FileReader(args[0]))) {
            List<String> fileLines = new ArrayList<>();

            String line;
            while ((line = inputFile.readLine()) != null) {
                fileLines.add(line);
            }

            return fileLines.toArray(new String[0]);
        } catch (Exception ex) {
            throw new ExecutionFailedException("ReadUnit execution failed: " + ex.getMessage(), ex);
        }

    }
}
