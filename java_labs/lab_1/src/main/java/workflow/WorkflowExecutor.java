package workflow;

import factory.UnitFactory;
import factory.exceptions.UnitFactoryException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import units.Unit;
import units.exceptions.UnitExecutionException;

public class WorkflowExecutor {
    private static final Logger logger = LogManager.getLogger(WorkflowExecutor.class);
    private WorkflowPlan _plan;

    public WorkflowExecutor(WorkflowPlan plan) {
        if (plan == null) {
            throw new IllegalArgumentException("plan is null");
        }
        _plan = plan;
    }

    public void work() {
        logger.info("Executing the workflow plan");
        try {
            String[] input = null;
            for (UnitInfo unitInfo: _plan) {
                Unit unit = UnitFactory.getInstance().getUnit(unitInfo.getUnitName());
                input = unit.execute(input, unitInfo.getUnitArgs());
            }
        } catch (UnitFactoryException | UnitExecutionException ex) {
            logger.error("Failed to execute the working plan", ex);
            return;
        }
        logger.info("Execution is done");
    }
}
