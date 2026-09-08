package org.example;

import com.google.api.client.util.DateTime;
import com.google.api.services.calendar.Calendar;
import com.google.api.services.calendar.model.Event;
import com.google.api.services.calendar.model.EventDateTime;

import java.io.IOException;
import java.util.Date;

public class CalendarService {

    public void createBackupEvent(Calendar calendarService, int numberOfFiles, long dataSize) throws IOException {
        // Replace with your calendar ID
        String calendarId = "primary";

        // Create a new event
        Event event = new Event();
        event.setSummary("Backup Completed");
        event.setDescription("Backup has been made. Number of files: " + numberOfFiles + ", Data size: " + dataSize + " bytes.");

        // Set the event start time to the current time
        DateTime startDateTime = new DateTime(System.currentTimeMillis());
        EventDateTime start = new EventDateTime().setDateTime(startDateTime).setTimeZone("Your TimeZone");
        event.setStart(start);

        // Set the event end time to 1 hour later
        DateTime endDateTime = new DateTime(System.currentTimeMillis() + 3600000); // 1 hour later
        EventDateTime end = new EventDateTime().setDateTime(endDateTime).setTimeZone("Your TimeZone");
        event.setEnd(end);

        // Add the event to the calendar
        calendarService.events().insert(calendarId, event).execute();

        System.out.println("Backup event added to Google Calendar.");
    }

}
