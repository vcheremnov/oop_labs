package workflow;

import workflow.exceptions.InvalidFormatException;

import java.util.*;

public class WorkflowPlan implements Iterable<UnitInfo> {
    Map<Integer, UnitInfo> _units = new HashMap<>();
    List<Integer> _plan = new ArrayList<>();

    public WorkflowPlan(String[] configText) {
        WorkflowReader reader = new WorkflowReader(configText);
        reader.fillWorkflowPlan();
    }

    @Override
    public Iterator<UnitInfo> iterator() {
        return new Iterator<>() {
            private final Iterator<Integer> _iter = _plan.iterator();

            @Override
            public boolean hasNext() {
                return _iter.hasNext();
            }

            @Override
            public UnitInfo next() {
                Integer unitID = _iter.next();
                return _units.get(unitID);
            }
        };
    }

    private class WorkflowReader {
        String[] _configText;

        public WorkflowReader(String[] configText) {
            _configText = configText;
        }

        public void fillWorkflowPlan() {
            int workflowBeg = _parseDescriptionBlock(_configText);
            String[] workflowBlock = Arrays.copyOfRange(_configText, workflowBeg, _configText.length);
            _parseWorkflowBlock(workflowBlock);
        }

        private int _parseDescriptionBlock(String[] configText) {
            int descBeg = 1 + _searchFor("desc", configText, 0);
            int descEnd = _searchFor("csed", configText, descBeg);
            for (int lineNo = descBeg; lineNo < descEnd; ++lineNo) {
                _parseDescriptionRecord(configText[lineNo]);
            }

            return descEnd + 1;

        }

        private void _parseDescriptionRecord(String record) {
            String[] recordParts = record.split("=", 2);

            if (recordParts.length == 1) {
                throw new InvalidFormatException(record + ": \"=\" was not found");
            }

            String stringId = recordParts[0].replaceAll("\\s+", "");
            Integer id;
            try {
                id = Integer.parseInt(stringId);
            } catch (Exception ex) {
                throw new InvalidFormatException(record + " : Failed to parse unit id (" + stringId + ")");
            }
            if (_units.containsKey(id)) {
                throw new InvalidFormatException(record + ": Duplicate unit id (" + id + ")");
            }

            StringTokenizer tokenizer = new StringTokenizer(recordParts[1]);
            if (!tokenizer.hasMoreTokens()) {
                throw new InvalidFormatException(record + ": Unit name was not found");
            }
            String unitName = tokenizer.nextToken();

            List<String> argList = new ArrayList<>();
            while (tokenizer.hasMoreTokens()) {
                argList.add(tokenizer.nextToken());
            }

            UnitInfo unitInfo = new UnitInfo();
            unitInfo.setUnitName(unitName);
            unitInfo.setUnitArgs(argList.toArray(new String[0]));

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
                throw new InvalidFormatException(String.format("Invalid format: the keyword \"%s\" is missing", keyword));
            }

            return lineNo;
        }

        private void _parseWorkflowBlock(String[] workflowBlock) {
            StringBuilder lineConcat = new StringBuilder();
            for (String line : workflowBlock) {
                lineConcat.append(line);
            }
            String workflowLine = lineConcat.toString().replaceAll("\\s+", "");

            Scanner scanner = new Scanner(workflowLine);
            scanner.useDelimiter("->");

            List<Integer> idList = new ArrayList<Integer>();
            while (scanner.hasNext()) {
                if (!scanner.hasNextInt()) {
                    throw new InvalidFormatException("Invalid format: found not a number in the workflow line");
                }
                int id = scanner.nextInt();
                if (!_units.containsKey(id)) {
                    throw new InvalidFormatException("Invalid format of the workflow line: " +
                            "found a number not from the description block");
                }
                idList.add(id);
            }

            _plan = idList;
        }
    }
}
