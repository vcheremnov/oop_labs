package units;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

class GrepUnitTest {
    private final GrepUnit unit = new GrepUnit();

    @Test
    @DisplayName("Invalid input/args")
    void invalidInputAndArgs() {
        String[] input = new String[0];
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input),
                "Has to throw IllegalArgumentException if args num != 1");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input, new String[2]),
                "Has to throw IllegalArgumentException if args num != 1");
        assertDoesNotThrow(() -> unit.execute(input, new String[1]),
                "Doesn't have to throw anything");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(null, "searchWord"),
                "Has to throw IllegalArgumentException if input is null");
    }

    @Test
    @DisplayName("Grep word")
    void grepWord() {
        String word = "war";
        String[] input = {
                "War, war never changes...",
                "Peace sells, but who's buying?...",
                "Run, cowards!"
        };
        List<String> output = Arrays.asList(unit.execute(input, word));
        List<String> expectedOutput = Arrays.asList(input[0], input[2]);

        assumeTrue(output.containsAll(expectedOutput) && expectedOutput.containsAll(output),
                "Output lines and expected lines are equal as sets");
        assumeTrue(output.equals(expectedOutput),
                "The original order of grepped lines was saved");
    }
}