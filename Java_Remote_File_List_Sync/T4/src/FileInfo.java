import java.io.File;
import java.io.Serializable;
import java.util.Comparator;
import java.util.Date;
import java.util.Objects;

/**
 * This class encapsulates information about a file, including its last modified
 * date.
 * This is necessary because the `io.File` class does not store the last
 * modified date, but instead calculates it dynamically.
 */
public class FileInfo implements Serializable, Comparable<FileInfo> {
    private final String filePath;
    private final long fileLastModified;

    /**
     * Constructs FileInfo object to represent the specified file.
     *
     * @param file The file to represent
     */
    public FileInfo(File file) {
        filePath = file.getAbsolutePath();
        fileLastModified = file.lastModified();
    }

    /**
     * Compares two FileInfo objects to determine if they are equal.
     *
     * @param obj The object to compare against.
     * @return True if the objects are equal, false otherwise.
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null || getClass() != obj.getClass())
            return false;
        FileInfo info = (FileInfo) obj;
        return Objects.equals(filePath, info.filePath);
    }

    @Override
    public int hashCode() {
        return Objects.hash(filePath);
    }

    /**
     * CompareTo method to compare two FileInfo objects based on their file paths.
     *
     * @param obj the FileInfo object to compare to.
     * @return the comparison result.
     */
    @Override
    public int compareTo(FileInfo obj) {
        return filePath.compareTo(obj.filePath);
    }

    /**
     * Comparator that compares FileInfo objects based on their lastModified dates.
     */
    public static final Comparator<FileInfo> LAST_MODIFIED_COMPARATOR = new Comparator<FileInfo>() {
        @Override
        public int compare(FileInfo o1, FileInfo o2) {
            return Long.compare(o1.fileLastModified, o2.fileLastModified);
        }
    };

    @Override
    public String toString() {
        return filePath + " (" + (new Date(fileLastModified)) + ')';
    }

    public long getLastModified() {
        return fileLastModified;
    }

    public String getPath() {
        return filePath;
    }
}