package units;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import units.exceptions.UnitExecutionException;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import static org.junit.jupiter.api.Assertions.*;

class WriteUnitTest {
    private final WriteUnit unit = new WriteUnit();
    private final String filepath = "WriteUnitTest.txt";

    @Test
    @DisplayName("Invalid input/args")
    void invalidInputAndArgs() {
        String[] input = new String[0];
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input),
                "Has to throw IllegalArgumentException if args num != 1");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(input, new String[2]),
                "Has to throw IllegalArgumentException if args num != 1");
        assertDoesNotThrow(() -> unit.execute(input, filepath),
                "Hasn't throw anything");
        assertThrows(IllegalArgumentException.class, () -> unit.execute(null, "searchWord"),
                "Has to throw IllegalArgumentException if input is null");
    }

    @Test
    @DisplayName("Write & read test")
    void writeAndRead() {
        String[] writtenLines = {
                "To be, or not to be, that is the question",
                "Whether 'tis nobler in the mind to suffer",
                "The slings and arrows of outrageous fortune,",
                "Or to take Arms against a Sea of troubles,"
        };

        try {
            unit.execute(writtenLines, filepath);
        } catch (UnitExecutionException ex) {
            ex.printStackTrace();
            fail("Failed to execute the write unit");
        }

        String[] readLines = null;
        try (BufferedReader inputFile = new BufferedReader(new FileReader(filepath))) {
            readLines = inputFile.lines().toArray(String[]::new);
        } catch (IOException ex) {
            ex.printStackTrace();
            fail("Failed to read from the file");
        }

        assertArrayEquals(writtenLines, readLines, "Have to be equal");
    }
}