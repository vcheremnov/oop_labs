package units;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Arrays;

public class GrepUnit implements Unit {
    private static final Logger logger = LogManager.getLogger(GrepUnit.class);
    @Override
    public String[] execute(String[] input, String... args) {
        logger.info("Executing GrepUnit with args " + Arrays.asList(args));
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
