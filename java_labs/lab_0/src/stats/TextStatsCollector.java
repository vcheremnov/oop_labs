package stats;

import java.io.InputStream;
import java.io.OutputStream;

public interface TextStatsCollector {
    void collectData(InputStream is);

    void printData(OutputStream os);

    void resetData();
}
