*This project has been created as part of the 42 curriculum by sfelici.*

# Get Next Line

## Description

Get Next Line is a C function that reads and returns one line at a time from a file descriptor. Each successive call to `get_next_line()` returns the next line until there is nothing left to read, at which point it returns `NULL`. The function works both on regular files and on standard input, and correctly handles any `BUFFER_SIZE` value set at compile time.

The project introduces a key concept in C programming: **static variables** — variables that persist their value between function calls without being global.

The bonus part extends the function to handle multiple file descriptors simultaneously, maintaining independent reading state for each one.

## Instructions

### Compilation

The project must be compiled with the flags `-Wall -Wextra -Werror`. The `BUFFER_SIZE` can be set at compile time using the `-D` flag. If not specified, it defaults to `10`.

**Mandatory:**
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c your_main.c
```

**Bonus (multi-fd support):**
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c your_main.c
```

### Usage

```c
#include "get_next_line.h"
#include <fcntl.h>

int main(void)
{
    int   fd;
    char  *line;

    fd = open("file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Files

| File | Description |
|------|-------------|
| `get_next_line.c` | Core function |
| `get_next_line_utils.c` | Helper functions (`ft_strlen`, `ft_strchr`, `ft_strjoin`) |
| `get_next_line.h` | Header file |
| `get_next_line_bonus.c` | Bonus version with multi-fd support |
| `get_next_line_utils_bonus.c` | Helper functions for bonus |
| `get_next_line_bonus.h` | Header file for bonus |

## Algorithm

The function is split into three stages, each handled by a dedicated static helper:

**1. `reader()` — Fill the stash**

A static variable `stash` acts as a persistent buffer between calls. On each call, `reader()` checks whether `stash` already contains a newline. If it does, no read is needed and the stash is returned as-is. If not, it calls `read()` repeatedly with a buffer of `BUFFER_SIZE` bytes, appending each chunk to the stash via `ft_strjoin()`, until either a `\n` is found or the file ends.

This approach ensures we read as little as possible per call — once a newline is found, reading stops immediately, and the remainder stays in the stash for the next call.

**2. `extract_line()` — Extract the current line**

Once the stash is ready, `extract_line()` allocates and returns a new string containing everything up to and including the first `\n` (or up to the end of the stash if no `\n` is present, which handles files without a trailing newline).

**3. `cleaner()` — Trim the stash**

After the line is extracted, `cleaner()` shifts the stash forward past the consumed line, keeping only the remaining unread data for the next call. If nothing remains, it frees the stash and sets it to `NULL`.

**Why this design?**

This three-function split keeps responsibilities clearly separated and makes each part easy to reason about and test. The stash-based approach avoids seeking backwards in the file descriptor (which would require `lseek()`, which is forbidden) and works correctly regardless of the relationship between `BUFFER_SIZE` and the actual line length.

**Bonus — multi-fd support**

The bonus version replaces the single `static char *stash` with `static char *stash[MAX_FD]`, where `MAX_FD` is defined as `4096` in the header. Each file descriptor uses its own stash slot (`stash[fd]`), so reading states are fully independent. This satisfies the bonus requirement of using only one static variable (an array counts as one variable) while supporting concurrent reading from multiple file descriptors.

## Resources

I don't remember
