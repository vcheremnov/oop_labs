package units;

import java.util.Arrays;

public class GrepUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (input == null) {
            throw new IllegalArgumentException("GrepUnit error: input is null");
        }
        if (args.length != 1) {
            throw new IllegalArgumentException("GrepUnit error: exactly one search word is expected");
        }

       return Arrays.stream(input)
                    .filter(line -> line.contains(args[0]))
                    .toArray(String[]::new);
    }
}
