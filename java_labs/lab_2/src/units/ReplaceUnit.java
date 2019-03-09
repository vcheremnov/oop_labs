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

        String target = args[0], replacement = args[1];
        for (int i = 0; i < input.length; ++i) {
            input[i] = input[i].replace(target, replacement);
        }

        return input;
    }
}
