package org.example;

import com.google.api.client.http.FileContent;
import com.google.api.services.drive.Drive;
import com.google.api.services.drive.model.File;
import com.google.api.services.drive.model.FileList;

import java.io.IOException;
import java.nio.file.*;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class DriveService {
    private int uploadCount = 0;
    public void synchronizeFiles(Drive service) throws IOException {
        String folderName = "log";
        String folderId = getOrCreateFolder(service, folderName);
        Set<String> driveFileNames = getDriveFileNames(service, folderId);
        Path localDirectoryPath = Paths.get("C:/Users/sarao/OneDrive/Skrivbord/log/");

        uploadNewFiles(service, localDirectoryPath, folderId, driveFileNames);

        System.out.println("File synchronization completed successfully.");
        System.out.println("Count of files uploaded to Google Drive is: " + uploadCount);
    }

    private void uploadNewFiles(Drive service, Path localDirectoryPath, String folderId, Set<String> driveFileNames)
            throws IOException {
        Files.walk(localDirectoryPath)
                .filter(Files::isRegularFile)
                .filter(path -> !driveFileNames.contains(path.getFileName().toString()))
                .forEach(path -> {
                    try {
                        File fileMetadata = new File();
                        fileMetadata.setName(path.getFileName().toString());
                        fileMetadata.setParents(List.of(folderId));

                        FileContent mediaContent = new FileContent(null, path.toFile());
                        File uploadedFile = service.files().create(fileMetadata, mediaContent)
                                .setFields("id")
                                .execute();

                        System.out.println("File uploaded: " + path.getFileName() + ", ID: " + uploadedFile.getId());
                        uploadCount++;
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                });
    }


    private String getOrCreateFolder(Drive service, String folderName) throws IOException {
        FileList result = service.files().list()
                .setQ("mimeType='application/vnd.google-apps.folder' and name='" + folderName + "'")
                .execute();

        List<File> folders = result.getFiles();
        if (!folders.isEmpty()) {
            return folders.get(0).getId();
        } else {
            File folder = new File();
            folder.setName(folderName);
            folder.setMimeType("application/vnd.google-apps.folder");
            File createdFolder = service.files().create(folder).execute();
            return createdFolder.getId();
        }
    }

    private Set<String> getDriveFileNames(Drive service, String folderId) throws IOException {
        Set<String> fileNames = new HashSet<>();
        FileList result = service.files().list()
                .setQ("'" + folderId + "' in parents")
                .setFields("files(name)")
                .execute();

        List<File> files = result.getFiles();
        if (files != null) {
            files.forEach(file -> fileNames.add(file.getName()));
        }
        return fileNames;
    }
}
