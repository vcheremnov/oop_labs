package stats;

import datawriter.DataWriter;
import parser.Parser;

import java.util.*;

public class WordStatsCollector {
    private int _totalWords = 0;
    private List<WordItem> _wordList = null;
    private Comparator<WordItem> _comparator;

    public WordStatsCollector() {
        this((o1, o2) -> {
            if (o1.getCount().equals(o2.getCount())) {
                return o1.getWord().compareTo(o2.getWord());
            }
            return o2.getCount().compareTo(o1.getCount());
        });
    }

    public WordStatsCollector(Comparator<WordItem> comparator) {
        setComparator(comparator);
    }

    public void setComparator(Comparator<WordItem> comparator) {
        _comparator = comparator;
    }

    public void collectWords(Parser parser) {
        resetWords();
        Map<String, Integer> wordCount = _countWords(parser);
        _fillWordList(wordCount);
        sortWords();
    }

    private Map<String, Integer> _countWords(Parser parser) {
        Map<String, Integer> wordCount = new HashMap<>();

        try {
            for (String word; (word = parser.getNextToken()) != null; ++_totalWords) {
                word = word.toLowerCase();
                int counter = wordCount.getOrDefault(word, 0) + 1;
                wordCount.put(word, counter);
            }
        } catch (Exception ex) {
            System.err.println("Error: " + ex.getMessage());
        }

        return wordCount;
    }

    private void _fillWordList(Map<String, Integer> wordCount) {
        _wordList = new ArrayList<>(wordCount.size());
        wordCount.forEach((word, count) -> _wordList.add(new WordItem(word, count)));
    }

    public void printWordStats(DataWriter dataWriter) {
        String[] wordStats = new String[3];
        for (WordItem wordItem : _wordList) {
            wordStats[0] = wordItem.getWord();
            wordStats[1] = wordItem.getCount().toString();
            wordStats[2] = String.format("%g", (double) wordItem.getCount() / _totalWords * 100.0);
            dataWriter.write(wordStats);
        }
    }

    public void sortWords() {
        _wordList.sort(_comparator);
    }

    public List<WordItem> getWords() {
        return _wordList;
    }

    public void resetWords() {
        _wordList = null;
        _totalWords = 0;
    }

    static public class WordItem {
        private String _word;
        private Integer _count;

        WordItem(String word, Integer count) {
            _word = word;
            _count = count;
        }

        String getWord() {
            return _word;
        }

        Integer getCount() {
            return _count;
        }
    }
}
