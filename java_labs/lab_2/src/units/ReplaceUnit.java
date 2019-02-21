package units;

public class ReplaceUnit implements Unit {
    @Override
    public String[] execute(String[] input, String... args) {
        if (input == null) {
            throw new IllegalArgumentException("ReplaceUnit error: input is null");
        }
        if (args.length != 2) {
            throw new IllegalArgumentException("ReplaceUnit error: exactly two words are expected");
        }

        for (int i = 0; i < input.length; ++i) {
            input[i] = input[i].replace(args[0], args[1]);
        }

        return input;
    }
}
