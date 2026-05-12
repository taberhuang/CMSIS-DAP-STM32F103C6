/* -----------------------------------------------------------------------------
 * syscalls.c - minimal newlib stubs
 *
 * Replaces libnosys.a. The stubs in libnosys emit "is not implemented" linker
 * warnings for _close/_lseek/_read/_write whenever they are referenced. By
 * providing equivalent no-op stubs here and removing --specs=nosys.specs from
 * the link line, those warnings disappear.
 *
 * Wiring: CMakeLists.txt links with --specs=nano.specs only (newlib-nano);
 * libnosys is intentionally NOT pulled in.
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#undef errno
extern int errno;

/* The linker script's _user_heap_stack section defines `end` as the start of
 * the heap. */
extern char end asm("end");
static char *heap_end;

void *_sbrk(int incr)
{
    if (heap_end == 0) {
        heap_end = &end;
    }
    char *prev_heap_end = heap_end;
    heap_end += incr;
    return (void *)prev_heap_end;
}

int _close(int file)        { (void)file; return -1; }
int _fstat(int file, struct stat *st) { (void)file; st->st_mode = S_IFCHR; return 0; }
int _isatty(int file)       { (void)file; return 1; }
int _lseek(int file, int ptr, int dir) { (void)file; (void)ptr; (void)dir; return 0; }
int _read(int file, char *ptr, int len) { (void)file; (void)ptr; (void)len; return 0; }
int _write(int file, char *ptr, int len) { (void)file; (void)ptr; return len; }
int _getpid(void)           { return 1; }
int _kill(int pid, int sig) { (void)pid; (void)sig; errno = EINVAL; return -1; }
void _exit(int status)      { (void)status; while (1) { } }
