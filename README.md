# kv-store

A simple C library aimed to store and manage key-value pairs in a plain-text file.

It allows to:
* Load multiple databases from a text file to memory
* Select the data structure that will store a database
* Apply CRUD operations to loaded databases

## Configuration
Fetch this repository and make it available using CMake:

```cmake
include(FetchContent)

FetchContent_Declare(kv_store
  GIT_REPOSITORY https://github.com/rijegaro287/kv-store.git
  GIT_TAG        origin/main
)

FetchContent_MakeAvailable(kv_store)

...

target_link_libraries(executable PRIVATE kv_store)
```

## How to use
Include the ```kv_controller.h``` header file:

```c
include "kv_controller.h"

int main() {
  return 0;
}

```

### Load a database

```c
#include "kv_controller.h"

int main() {
  db_t *db;
  if (load_db(db, "") < 0) {
    printf("Failed to load database\n");
  }
  return 0;
}

```

