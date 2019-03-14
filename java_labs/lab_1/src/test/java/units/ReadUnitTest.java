package units;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import units.exceptions.UnitExecutionException;

import java.io.*;

import static org.junit.jupiter.api.Assertions.*;

class ReadUnitTest {
    private final ReadUnit unit = new ReadUnit();
    private final String filepath = "ReadUnitTest.txt";

    @Test
    @DisplayName("Invalid args")
    void invalidArgs() {
        String[] input = new String[0];
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input),
                "Has to throw IllegalArgumentException if args num != 1");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input, new String[2]),
                "Has to throw IllegalArgumentException if args num != 1");
        assertDoesNotThrow(() -> unit.execute(input, filepath),
                "Doesn't have to throw anything");
    }

    @Test
    @DisplayName("Nonexistent file test")
    void openNonexistentFile() {
        assertThrows(UnitExecutionException.class,
                () -> unit.execute(new String[0], "a_file_that_does_not_exist.txt"),
                "Has to throw UnitExecutionException exception");
    }

    @Test
    @DisplayName("Read file test")
    void readFileTest() {
        String[] expectedLines = null;
        try (BufferedReader inputFile = new BufferedReader(new FileReader(filepath))) {
            expectedLines = inputFile.lines().toArray(String[]::new);
        } catch (IOException ex) {
            ex.printStackTrace();
            fail("Failed to read from the file");
        }

        String[] outputLines = null;
        try {
            outputLines = unit.execute(null, filepath);
        } catch (UnitExecutionException ex) {
            ex.printStackTrace();
            fail("Failed to execute the read unit");
        }

        assertArrayEquals(outputLines, expectedLines, "Have to be equal");
    }

}