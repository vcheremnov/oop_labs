import workflow.UnitInfo;
import workflow.WorkflowExecutor;
import workflow.WorkflowPlan;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainClass {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Usage: java ... MainClass.java <workflow file>");
            System.exit(-1);
        }

        WorkflowPlan plan;
        try (BufferedReader file = new BufferedReader(new FileReader(args[0]))) {

            List<String> fileLines = new ArrayList<>();

            String line;
            while ((line = file.readLine()) != null) {
                fileLines.add(line);
            }

            plan = new WorkflowPlan(fileLines.toArray(new String[0]));

        } catch (Exception ex) {
            ex.printStackTrace();
            return;
        }

        WorkflowExecutor executor = new WorkflowExecutor(plan);
        executor.work();

    }
}
