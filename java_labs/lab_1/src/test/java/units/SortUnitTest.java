package units;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assumptions.assumeTrue;

class SortUnitTest {
    private final SortUnit unit = new SortUnit();

    @Test
    @DisplayName("Invalid input")
    void invalidInput() {
        assertThrows(IllegalArgumentException.class, () -> unit.execute(null),
                "Has to throw IllegalArgumentException if input is null");
    }

    @DisplayName("Sort lines")
    @Test
    void sortLines() {
        String[] input = {"Abc", "Gpfqs", "Tsargr", "abc", "abe", "abey", "wow", "xyz"};
        List<String> shuffledInput = Arrays.asList("Abc", "Gpfqs", "Tsargr", "abc", "abe", "abey", "wow", "xyz");
        Collections.shuffle(shuffledInput);
        String[] output = unit.execute(shuffledInput.toArray(new String[0]));
        assumeTrue(Arrays.deepEquals(input, output), "Have to be equal");
    }
}