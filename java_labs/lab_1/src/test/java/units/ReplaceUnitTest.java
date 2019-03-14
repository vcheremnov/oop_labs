package units;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

class ReplaceUnitTest {
    private final ReplaceUnit unit = new ReplaceUnit();

    @Test
    @DisplayName("Invalid input/args")
    void invalidInputAndArgs() {
        String[] input = new String[0];
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input),
                "Has to throw IllegalArgumentException if args num != 2");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input, new String[3]),
                "Has to throw IllegalArgumentException if args num != 2");
        assertDoesNotThrow(() -> unit.execute(input, new String[2]),
                "Doesn't have to throw anything");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(null, "target", "replacement"),
                "Has to throw IllegalArgumentException if input is null");
    }

    @Test
    @DisplayName("Replace words")
    void replaceWords() {
        String[] input = {"Rating waiting test", "123 demonstrating the replace unit", "dating rating meeting"};
        String[] output = unit.execute(input, "rating", "replaced");
        String[] expectedOutput = {"Rating waiting test", "123 demonstreplaced the replace unit", "dating replaced meeting"};
        assumeTrue(Arrays.deepEquals(output, expectedOutput), "Have to be equal");
    }

    @Test
    @DisplayName("Empty string replacement")
    void emptyStringReplace() {
        String[] input = {"To be or", "not to be"};
        String[] output = unit.execute(input, "", "*");
        String[] expectedOutput = {"*T*o* *b*e* *o*r*", "*n*o*t* *t*o* *b*e*"};
        assumeTrue(Arrays.deepEquals(output, expectedOutput), "Have to be equal");
    }

}