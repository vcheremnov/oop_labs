package units;

import java.util.Arrays;

public class GrepUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (input == null) {
            throw new IllegalArgumentException("GrepUnit error: input is null");
        }
        if (args.length != 1) {
            throw new IllegalArgumentException("GrepUnit error: exactly one search substring is expected");
        }

        String requiredSubstring = args[0];
        return Arrays.stream(input)
                     .filter(line -> line.contains(requiredSubstring))
                     .toArray(String[]::new);
    }
}
