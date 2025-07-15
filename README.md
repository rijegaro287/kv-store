# kv-store

A simple C library aimed to store and manage key-value pairs in a plain-text file.

It allows to:
* Load a database from a text file to memory
* Select between a Hash Table and a Linked List to store the data in memory
* Apply CRUD operations to databases

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

## Basic Usage
Include the ```kv_controller.h``` header file:

```c
#include "kv_controller.h"

int main() {
  return 0;
}
```

### Create and Load a database
Allocates memory for a database and loads the contents of a file into it.

The available types of storage are a linked lists and hash tables, defined in the ```KV_STORAGE_STRUCTURE_LIST``` and ```KV_STORAGE_STRUCTURE_HASH``` constants.

```c
db_t *db = create_db(KV_STORAGE_STRUCTURE_LIST);
if (load_db(db, "test.db") < 0) {
  printf("Failed to load database\n");
}
```

### Insert an entry
Creates and inserts an entry containing a key, a value and a datatype.

```c
db_entry_t *entry = create_entry("key1", "127", "int8");
if (entry == NULL) {
  printf("Failed to create an entry\n");
}
insert_entry(db, entry);
```

### Update an entry
Updates the value and type of an entry with the given key. If the key does not exist, it is created.

```c
if (put_entry(db, "key2", "123", "int8") < 0) {
  printf("Failed to put an entry in a database\n");
}
```

### Get an entry
Gets an entry with the given key from a database.

```c
db_entry_t *entry_ = get_entry(db, "key1");
if (entry_ == NULL) { 
  printf("Failed to get an entry from a database\n");
}
```

### Delete an entry
Deletes an entry with the given key from a database.

```c
if (delete_entry(db, "key1") < 0) {
  printf("Failed to delete and entry from a database\n");
}
```

### Save a database
Saves the current state of the loaded database.

```c
if (save_db(db, "test.db") < 0) {
  printf("Failed to save a database\n");
}
```

## Database File Format
Each entry is stored using this default format ```<datatype>:<key>=<value>;```

The delimiters for each parameter are defined in the ```KV_PARSER_TYPE_DELIMITER```, ```KV_PARSER_KEY_DELIMITER``` and ```KV_PARSER_VALUE_DELIMITER``` constants.

## API Documentation
Click [here](https://rijegaro287.github.io/kv-store/dir_d44c64559bbebec7f509842c48db8b23.html) to see a list of available header files and the functions they include.


## Running Tests
You can run unit tests by building the project and running ctest:

```
git clone https://github.com/rijegaro287/kv-store
cd kv-store
mkdir -p ./build
cd ./build
cmake ..
make
ctest --output-on-failure -V
```
