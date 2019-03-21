package units;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Arrays;

public class ReplaceUnit implements Unit {
    private static final Logger logger = LogManager.getLogger(ReplaceUnit.class);
    @Override
    public String[] execute(String[] input, String... args) {
        logger.info("Executing ReplaceUnit with args " + Arrays.asList(args));
        if (input == null) {
            throw new IllegalArgumentException("ReplaceUnit error: input is null");
        }
        if (args.length != 2) {
            throw new IllegalArgumentException("ReplaceUnit error: exactly two words are expected");
        }

        String target = args[0], replacement = args[1];
        for (int i = 0; i < input.length; ++i) {
            input[i] = input[i].replace(target, replacement);
        }

        return input;
    }
}
