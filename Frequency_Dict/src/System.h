#pragma once

#include <cstddef>
#include <sys/types.h>

// Write memory in @a chunk to the @a fd till the @a chunk_end
// (so chunk_end - chunk bytes are written). Returns value < 0
// in case of error (except for EINTR); number of written bytes
// otherwise.
ssize_t
file_write_chunk(int fd, void *chunk, void *chunk_end);

// Writes the size of file @a fd file descriptor is pointing to
// to @a size output param. Returns 0 on success, < 0 on system error.
int
file_size(int fd, std::size_t *size);

// mmap the whole file with given FD with MAP_PRIVATE; mmaped size
// is returned via @a size param. Memory is available for writing.
// Returns nullptr in case of system fail (e.g. no enough memory).
void *
file_mmap(int fd, std::size_t *size);
