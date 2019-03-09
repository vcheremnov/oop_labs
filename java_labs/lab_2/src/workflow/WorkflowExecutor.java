package workflow;

import factory.UnitFactory;
import units.Unit;

public class WorkflowExecutor {
    private WorkflowPlan _plan;

    public WorkflowExecutor(WorkflowPlan plan) {
        if (plan == null) {
            throw new IllegalArgumentException("plan is null");
        }
        _plan = plan;
    }

    public void work() {
        try {
            String[] input = null;
            for (UnitInfo unitInfo: _plan) {
                Unit unit = UnitFactory.getInstance().getUnit(unitInfo.getUnitName());
                input = unit.execute(input, unitInfo.getUnitArgs());
            }
        } catch (RuntimeException ex) {
            System.err.println(ex.getMessage());
        }
    }
}
