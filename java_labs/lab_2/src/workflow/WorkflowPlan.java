package workflow;

import workflow.exceptions.InvalidFormatException;

import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class WorkflowPlan implements Iterable<UnitInfo> {
    private Map<String, UnitInfo> _units = new HashMap<>();
    private List<String> _plan = new ArrayList<>();

    public WorkflowPlan(String[] workflowConfig) {
        WorkflowParser parser = new WorkflowParser();
        parser.fillWorkflowPlan(workflowConfig);
    }

    @Override
    public Iterator<UnitInfo> iterator() {
        return new Iterator<>() {
            private final Iterator<String> _iter = _plan.iterator();

            @Override
            public boolean hasNext() {
                return _iter.hasNext();
            }

            @Override
            public UnitInfo next() {
                String unitID = _iter.next();
                return _units.get(unitID);
            }
        };
    }

    private class WorkflowParser {
        Matcher _descRecordMatcher;
        Matcher _workflowLineMatcher;

        public WorkflowParser() {
            // compile regex for description record format recognition (<id> = <word1> <word2> ....)
            Pattern recordPattern = Pattern.compile("\\s*\\d+\\s*=(\\s*\\S+\\s*)+");
            _descRecordMatcher = recordPattern.matcher("");
            // workflow line regex (id_1->id_2->...->id_n or empty string)
            Pattern workflowPattern = Pattern.compile("(^$)|(\\d+(->\\d+)*)");
            _workflowLineMatcher = workflowPattern.matcher("");
        }

        public void fillWorkflowPlan(String[] workflowConfig) {
            _units.clear();
            _plan.clear();

            int workflowBeg = _parseDescriptionBlock(workflowConfig);

            // concatenate lines in the workflow block
            int totalLength = 0;
            for (int lineNo = workflowBeg; lineNo < workflowConfig.length; ++lineNo) {
                totalLength += workflowConfig[lineNo].length();
            }
            StringBuilder lineConcat = new StringBuilder(totalLength);
            for (int lineNo = workflowBeg; lineNo < workflowConfig.length; ++lineNo) {
                lineConcat.append(workflowConfig[lineNo]);
            }
            String workflowLine = lineConcat.toString().replaceAll("\\s+", "");

            _parseWorkflowLine(workflowLine);
        }

        private int _parseDescriptionBlock(String[] descBlock) {
            int descBeg = 1 + _searchFor("desc", descBlock, 0);
            int descEnd = _searchFor("csed", descBlock, descBeg);
            for (int lineNo = descBeg; lineNo < descEnd; ++lineNo) {
                _parseDescriptionRecord(descBlock[lineNo]);
            }

            return descEnd + 1;

        }

        private void _parseDescriptionRecord(String descRecord) {
            _descRecordMatcher.reset(descRecord);
            if (!_descRecordMatcher.matches()) {
                throw new InvalidFormatException(descRecord + ": invalid format; " +
                        "must be \"unit_id = unit_name args...\"");
            }

            // split into two parts: [unit id] and [unit name, unit args]
            String[] recordParts = descRecord.split("=", 2);

            // parse unit id
            String id = recordParts[0].replaceAll("\\s+", "");
            if (_units.containsKey(id)) {
                throw new InvalidFormatException(descRecord + ": Duplicate unit id (" + id + ")");
            }

            // parse unit name
            StringTokenizer tokenizer = new StringTokenizer(recordParts[1]);
            String unitName = tokenizer.nextToken();

            // parse unit args
            List<String> argList = new ArrayList<>();
            while (tokenizer.hasMoreTokens()) {
                argList.add(tokenizer.nextToken());
            }

            // fill info about the unit
            UnitInfo unitInfo = new UnitInfo();
            unitInfo.setUnitName(unitName);
            unitInfo.setUnitArgs(argList.toArray(new String[0]));

            // add to the units database
            _units.put(id, unitInfo);
        }

        private int _searchFor(String keyword, String[] lines, int startPos) {
            int lineNo = startPos;
            while (lineNo < lines.length) {
                String line = lines[lineNo].replaceAll("\\s+", "");
                if (line.equals(keyword)) {
                    break;
                }
                ++lineNo;
            }
            if (lineNo == lines.length) {
                throw new InvalidFormatException("Invalid format of the workflow config file: " +
                        "the keyword \"" + keyword + "\" is missing");
            }

            return lineNo;
        }

        private void _parseWorkflowLine(String workflowLine) {
            _workflowLineMatcher.reset(workflowLine);
            if (!_workflowLineMatcher.matches()) {
                throw new InvalidFormatException("Invalid format of the workflow line; " +
                        "must be \"id_k1->id_k2->...->id_km\", where id_ki is from the description block");
            }

            String[] idArray = workflowLine.split("->");
            for (String id: idArray) {
                if (!_units.containsKey(id)) {
                    throw new InvalidFormatException("Invalid format of the workflow line: " +
                            "id \"" + id + "\" is not from the description block");
                }
            }

            _plan = Arrays.asList(idArray);
        }
    }
}
