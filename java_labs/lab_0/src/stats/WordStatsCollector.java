package stats;

import parser.Parser;
import parser.WordParser;

import java.io.*;
import java.util.*;

public class WordStatsCollector implements TextStatsCollector {
    private List<WordItem> _wordList = null;
    private final Comparator<WordItem> _comparator;

    public WordStatsCollector() {
        this((o1, o2) -> {
            if (o1.getCount().equals(o2.getCount())) {
                return o1.getWord().compareTo(o2.getWord());
            }
            return o2.getCount().compareTo(o1.getCount());
        });
    }

    public WordStatsCollector(Comparator<WordItem> comparator) {
        _comparator = comparator;
    }

    @Override
    public void collectData(InputStream is) {
        resetData();
        Map<String, Integer> wordCount = _countWords(is);
        _fillWordList(wordCount);
        _wordList.sort(_comparator);
    }

    private Map<String, Integer> _countWords(InputStream is) {
        Map<String, Integer> wordCount = new HashMap<>();
        Parser parser = new WordParser(is);

        try {
            String word;
            while ((word = parser.getNextToken()) != null) {
                word = word.toLowerCase();
                Integer counter = wordCount.getOrDefault(word, 0) + 1;
                wordCount.put(word, counter);
            }
        } catch (IOException ex) {
            System.err.println("Error: " + ex.getMessage());
        }

        return wordCount;
    }

    private void _fillWordList(Map<String, Integer> wordCount) {
        _wordList = new ArrayList<>(wordCount.size());
        wordCount.forEach((word, count) -> _wordList.add(new WordItem(word, count)));
    }

    @Override
    public void printData(OutputStream os) {
        Integer totalCount = 0;
        // count total number of words
        for (WordItem item : _wordList) {
            totalCount += item.getCount();
        }
        // print stats in csv format
        try {
            PrintWriter outStream = new PrintWriter(os, true);
            for (WordItem record : _wordList) {
                outStream.printf("%s,%d,%g\n", record.getWord(), record.getCount(),
                        (double) record.getCount() / totalCount);
            }
        } catch (Exception ex) {
            System.err.println("Error: " + ex.getMessage());
        }
    }

    @Override
    public void resetData() {
        _wordList = null;
    }
}
