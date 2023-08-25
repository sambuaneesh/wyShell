#include "headers.h"
#include "tools.h"

char *past_events[MAX_PAST_EVENTS];
int num_past_events = 0;

void loadPastEvents()
{
    FILE *file = fopen(PAST_EVENTS_FILE, "r");
    if (file != NULL)
    {
        char line[MAX_INPUT_LENGTH];
        while (fgets(line, sizeof(line), file) != NULL && num_past_events < MAX_PAST_EVENTS)
        {
            line[strcspn(line, "\n")] = '\0';
            past_events[num_past_events++] = strdup(line);
        }
        fclose(file);
    }
}

void savePastEvents()
{
    FILE *file = fopen(PAST_EVENTS_FILE, "w");
    if (file != NULL)
    {
        for (int i = 0; i < num_past_events; i++)
        {
            fprintf(file, "%s\n", past_events[i]);
        }
        fclose(file);
    }
}

void addToPastEvents(const char *command)
{
    if (strstr(command, "pastevents") == command)
        return;
    if (strcmp(command, past_events[num_past_events - 1]) == 0)
        return;
    if (num_past_events == MAX_PAST_EVENTS)
    {
        free(past_events[0]);
        for (int i = 0; i < num_past_events - 1; i++)
        {
            past_events[i] = past_events[i + 1];
        }
        num_past_events--;
    }
    past_events[num_past_events++] = strdup(command);
    savePastEvents();
}

void displayPastEvents()
{
    for (int i = 0; i < num_past_events; i++)
    {
        printf("%d: %s\n", i + 1, past_events[i]);
    }
}

void purgePastEvents()
{
    for (int i = 0; i < num_past_events; i++)
    {
        free(past_events[i]);
    }
    num_past_events = 0;
    savePastEvents();
}

void pastevents(Command *cmd)
{
    if (cmd->argc > 3)
        fprintf(stderr, "pastevents: Too many arguments\n");
    else if (cmd->argc == 1)
        displayPastEvents();
    else if (strcmp(cmd->argv[1], "purge") == 0)
        purgePastEvents();
    else
        fprintf(stderr, "pastevents: Invalid Usage\n");
}