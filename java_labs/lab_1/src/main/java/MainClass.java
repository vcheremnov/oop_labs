
import workflow.WorkflowExecutor;
import workflow.WorkflowPlan;

import java.io.BufferedReader;
import java.io.FileReader;

public class MainClass {
    private static final int REQUIRED_ARGS_NUM = 1;
    private static final int ERROR_CODE = -1;

    public static void main(String[] args) {
        if (args.length != REQUIRED_ARGS_NUM) {
            System.err.println("Usage: java " + MainClass.class.getName() + " <main.workflow file>");
            System.exit(ERROR_CODE);
        }

        WorkflowPlan plan = null;
        try (BufferedReader file = new BufferedReader(new FileReader(args[0]))) {
            String[] workflowConfig = file.lines().toArray(String[]::new);
            plan = new WorkflowPlan(workflowConfig);
        } catch (Exception ex) {
            ex.printStackTrace();
            System.exit(ERROR_CODE);
        }

        WorkflowExecutor executor = new WorkflowExecutor(plan);
        executor.work();

    }
}
