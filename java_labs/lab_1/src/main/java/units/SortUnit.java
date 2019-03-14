package units;

import java.util.Arrays;

public class SortUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (input == null) {
            throw new IllegalArgumentException("SortUnit error: input is null");
        }

        Arrays.sort(input);
        return input;
    }
}
