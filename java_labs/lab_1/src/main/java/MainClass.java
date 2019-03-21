import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import workflow.WorkflowExecutor;
import workflow.WorkflowPlan;

import java.io.BufferedReader;
import java.io.FileReader;

public class MainClass {
    private static final int REQUIRED_ARGS_NUM = 1;
    private static final int ERROR_CODE = -1;

    private static final Logger logger = LogManager.getLogger(MainClass.class);

    public static void main(String[] args) {
        logger.info("Startup");
        if (args.length != REQUIRED_ARGS_NUM) {
            logger.warn("Invalid program arguments were passed");
            System.err.println("Usage: java " + MainClass.class.getName() + " <main.workflow file>");
            logger.info("Premature shutdown");
            System.exit(ERROR_CODE);
        }

        logger.info("Opening workflow configuration file");
        WorkflowPlan plan = null;
        try (BufferedReader file = new BufferedReader(new FileReader(args[0]))) {
            logger.info("Reading the file");
            String[] workflowConfig = file.lines().toArray(String[]::new);
            logger.info("Filling a workflow plan");
            plan = new WorkflowPlan(workflowConfig);
        } catch (Exception ex) {
            logger.fatal(String.format("Failed to open workflow configuration file: %s", ex.toString()));
            logger.info("Premature shutdown");
            System.exit(ERROR_CODE);
        }

        WorkflowExecutor executor = new WorkflowExecutor(plan);
        executor.work();
        logger.info("Shutdown");
    }
}
