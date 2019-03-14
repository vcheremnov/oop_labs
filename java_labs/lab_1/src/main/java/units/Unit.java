package units;

import units.exceptions.UnitExecutionException;

public interface Unit {

String[] execute(String[] input, String ...args) throws UnitExecutionException;

}
