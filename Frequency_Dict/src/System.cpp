#include "System.h"

#include <cassert>

#include <errno.h>
#include <stdio.h>
#include <string.h> //strerror()
#include <unistd.h> //write

#include <sys/mman.h> //mmap()/munmap()
#include <sys/types.h> //ssize_t
#include <sys/stat.h> //fstat()

ssize_t
file_write_chunk(int fd, void *chunk, void *chunk_end)
{
	assert(fd >= 0 && chunk_end >= chunk);
	size_t to_write_bytes = (size_t)((char *)chunk_end - (char *)chunk);
	size_t has_written = 0;
	while (to_write_bytes > 0) {
		ssize_t rc = ::write(fd, (char *)chunk + has_written, to_write_bytes);
		if (rc < 0) {
				if (errno == EINTR)
					continue;
			::fprintf(stderr, "write() has failed, written only %zu bytes: %s \n", has_written, ::strerror(errno));
			return -1;
		}
		has_written += rc;
		assert(to_write_bytes >= (size_t) rc);
		to_write_bytes -= rc;
	}
	assert(to_write_bytes == 0);
	return has_written;
}

int
file_size(int fd, std::size_t *size)
{
	struct stat sb;
	if (::fstat(fd, &sb) != 0) {
		::fprintf(stderr, "Failed to call fstat on fd %d: %s\n", fd, ::strerror(errno));
		return -1;
	}
	*size = sb.st_size;
	return 0;
}

void *
file_mmap(int fd, std::size_t *size)
{
	if (file_size(fd, size) != 0)
		return nullptr;
	// We specify PROT_WRITE since we are going to lowercase words right in mmaped
	// memory. It won't affect original file since we are using MAP_PRIVATE.
	void *mem = ::mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (mem == MAP_FAILED) {
		::fprintf(stderr, "Failed to mmap input file: %s\n", ::strerror(errno));
		return nullptr;
	}
	return mem;
}
