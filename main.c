#include <stdio.h>
#include <string.h>
#include <time.h>

enum TaskPriority {
  HIGH,
  MEDIUM,
  LOW,
  INVALID
};

struct MapPriority {
  const char* name;
  enum TaskPriority value;
};

struct MapPriority priority_table[] = {
  {"high", HIGH},
  {"medium", MEDIUM},
  {"low", LOW},
};

struct Task {
  char name[50];
  char description[200];
  enum TaskPriority priority;
  struct tm date;
};

struct FileContent {
  struct Task* tasks;
  struct tm creation_date;
  struct tm last_update;
};

enum TaskPriority string_to_priority(const char* str) {
  size_t num_elements = sizeof(priority_table) / sizeof(priority_table[0]);

  for (size_t i = 0; i < num_elements; i++) {
    if (strcmp(str, priority_table[i].name) == 0) {
      return priority_table[i].value;
    }
  }
  return INVALID;
}

void print_file_content(const char* file_name) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror("File %s doesn't exist in current workdir");
    return;
  }
  
  int c;
  while((c = fgetc(file)) != EOF) {
    putchar(c);
  }

  fclose(file);
}

void create_persistant_file() {
  FILE* fptr;
  
  // Check if file already exists
  FILE* f_check = fopen("persistance.json", "r");
  if (f_check != NULL) {
    perror("Persistance file already exists, cannot recreate it");
    return;
  }

  // Register empty persistance file in working directory
  fptr = fopen("persistance.json", "w");
  // Store it's metadata
  struct tm *today;
  time_t stamp = time(NULL);
  today = localtime(&stamp);

  struct FileContent file_content;
  file_content.tasks = NULL;
  file_content.creation_date = *today;
  memset(&file_content.last_update, 0, sizeof(struct tm));

  fclose(fptr);
}

void create_new_task() {
  struct Task tk;
  char buffer[200];

  printf("Insert Task name: ");
  if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    // Change last char from '\n' to '\0'
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(tk.name, buffer, sizeof(tk.name) - 1);
    memset(buffer, 0, sizeof(buffer)); // Free buffer content
  } else {
    perror("Error reading entry");
    return;
  }

  printf("Insert a sort description: ");
  if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(tk.description, buffer, sizeof(tk.description) - 1);
    memset(buffer, 0, sizeof(buffer)); // Free buffer content
  } else {
    perror("Error reading entry");
    return;
  }

  printf("Insert the priority of the task: ");
  if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0';
    enum TaskPriority prio;
    memset(buffer, 0, sizeof(buffer)); // Free buffer content
  } else {
    perror("Error reading entry");
    return;
  }

}


int main(int argc, char* argv[]) {
  
  if (argc == 1) {
    printf("Usage: %s --flag task_name", argv[0]);
    return 1;
  }
   
  const char* flag = argv[1]; // Store the flag to be used

  // Register persistance file in current directory
  create_persistant_file();

  // Remove the -- chars from the flag for better iteration
  const char* flag_resized = flag;
  while (*flag_resized == '-') {
    flag_resized++;
  }

  // Determine what action should compute based on the given flag value
  char buffer[200]; 
  switch (*flag_resized) {
    case 'l':
      print_file_content("persistance.json");
      break;
    case 'i':
      // Read task metadata to create a new task
      printf("=== You want to create a new task ===\n");
            break;
    case 'u':
      break;
    case 'd':
      break;
  }

  return 0;
}
