package units;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Arrays;

public class SortUnit implements Unit {
    private static final Logger logger = LogManager.getLogger(SortUnit.class);
    @Override
    public String[] execute(String[] input, String... args) {
        logger.info("Executing SortUnit");
        if (input == null) {
            throw new IllegalArgumentException("SortUnit error: input is null");
        }

        Arrays.sort(input);
        return input;
    }
}
