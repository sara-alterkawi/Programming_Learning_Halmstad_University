import java.io.File;
import java.util.*;

/**
 * This class is designed to provide static methods for handling file information.
 */
public class FileTools {
    /**
     * Retrieves file information from the specified directory.
     *
     * @param path The path of the directory to fetch file information from.
     * @return A set of FileInfo objects.
     */
    public static Set<FileInfo> getAllFiles(String path) {
        File directory = new File(path);
        File[] files = directory.listFiles();
        if (files != null)
            return getAllFiles(files, 0, 0);
        return new TreeSet<>();
    }

    private static Set<FileInfo> getAllFiles(File[] file, int index, int level) {
        TreeSet<FileInfo> fileSet = new TreeSet<>();
        if (index == file.length)
            return fileSet;

        if (file[index].isFile())
            fileSet.add(new FileInfo(file[index]));

        else if (file[index].isDirectory()) {
            File[] files = file[index].listFiles();
            if (files != null)
                fileSet.addAll(getAllFiles(files, 0, level + 1));
        }
        fileSet.addAll(getAllFiles(file, index + 1, level));
        return fileSet;
    }

    /**
     * Retrieves deleted files by comparing two sets of file information.
     *
     * @param oldFileSet The previous set.
     * @param newFileSet The updated set.
     * @return A set of deleted files from oldFileSet.
     */
    public static Set<FileInfo> deletedFiles(Set<FileInfo> oldFileSet, Set<FileInfo> newFileSet) {
        TreeSet<FileInfo> deletedSet = new TreeSet<>(oldFileSet);
        deletedSet.removeAll(newFileSet);
        return deletedSet;
    }

    /**
     * Retrieves added files by comparing two sets of file information.
     *
     * @param oldFileSet The previous set.
     * @param newFileSet The updated set.
     * @return A set of added files to newFileSet.
     */
    public static Set<FileInfo> addedFiles(Set<FileInfo> oldFileSet, Set<FileInfo> newFileSet) {
        TreeSet<FileInfo> addedSet = new TreeSet<>(newFileSet);
        addedSet.removeAll(oldFileSet);
        return addedSet;
    }

    /**
     * Searches for updated files when the last modified date is different.
     *
     * @param oldFileSet The previous set.
     * @param newFileSet The updated set.
     * @return A set of updated files from oldFileSet to newFileSet.
     */
    public static Set<FileInfo> updatedFiles(Set<FileInfo> oldFileSet, Set<FileInfo> newFileSet) {
        TreeMap<String, Long> oldFileMap = new TreeMap<>();
        for (FileInfo info : oldFileSet)
            oldFileMap.put(info.getPath(), info.getLastModified());

        TreeMap<String, Long> newMap = new TreeMap<>();
        for (FileInfo info : newFileSet)
            newMap.put(info.getPath(), info.getLastModified());

        TreeSet<FileInfo> updatedSet = new TreeSet<>();
        for (FileInfo info : newFileSet) {
            String path = info.getPath();
            if (oldFileMap.containsKey(path) &&
                    !newMap.get(path).equals(oldFileMap.get(path)))
                updatedSet.add(info);
        }
        return updatedSet;
    }

}