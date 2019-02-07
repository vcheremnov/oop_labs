package stats;

class WordItem {
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
