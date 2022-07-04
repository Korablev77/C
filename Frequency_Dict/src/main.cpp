// Feel free to steal code from this file as it doesn't protected
// with any license (due to the lack of any value).

#include <errno.h> // errno
#include <fcntl.h> // open()
#include <unistd.h> // ftruncate(), close()
#include <sysexits.h> // return codes from main
#include <string.h> // strerror()
#include "sys/mman.h" // munmap()

#include "PerfTimer.h"
#include "ScopedGuard.h"
#include "System.h"
#include "Words.h"

// The task is following: given text document (ASCII), calculate freqeunces for each appearing word.
// The solution is trivial: iterate over all words in the file and count them using hash table (as DS featuring
// O(1) lookup). Then move elements to a vector and sort them.
// Key optimizations:
// - Use mmap to read the file (assuming the file fits into ram; otherwise we probably should fallback to
//   old-but-good page read; fallback is NOT implemented to keep things simple);
// - Instead of using std::string just keep pointer to mmaped memory and words's length to avoid
//   any copies (probably std::string_view should be fine as well);
// - Move elements of hash table (pairs) to vector (instead of copying);
// - Use open-adressing hash table (hopscotch map).

//TODO: things which can improve perf (failed to test them as I've met deadlines):
// - use vector instructions;
// - use custom allocator (smth like region allocator which never releases memory);
// - mmap output file as well;
// - Use faster alternative to snprintf() to convert number to strings;

namespace {
	enum {
		// input+output filenames + exec name
		ARGC_COUNT = 3,
	};

/////////////////////////////////// Entry Point ///////////////////////////////////////////////////////////////////////

	int
	words_process(const char *input_filename, const char *output_filename)
	{
		int input_fd = ::open(input_filename, O_RDONLY, 0440);
		if (input_fd < 0) {
			::fprintf(stderr, "Can't open input file %s: %s\n", input_filename, ::strerror(errno));
			return -1;
		}
		auto close_input = create_scoped_guard([=] {::close(input_fd);});

		int output_fd = ::open(output_filename, O_RDWR | O_CREAT, 0640);
		if (output_fd < 0) {
			::fprintf(stderr, "Can't open output file %s: %s\n", output_filename, ::strerror(errno));
			return -1;
		}
		auto close_output = create_scoped_guard([=] {::close(output_fd);});

		// Just in case let's erase previous file content.
		if (::ftruncate(output_fd, 0) < 0) {
			::fprintf(stderr, "Failed to truncate file %s: %s\n", output_filename, ::strerror(errno));
			return -1;
		}

		std::size_t mem_size = 0;
		char *mem = static_cast<char *> (file_mmap(input_fd, &mem_size));
		if (mem == nullptr)
			return -1;
		auto free_mem = create_scoped_guard([=] {::munmap(mem, mem_size);});

		// Step 1: fill in hash table with word frequences.
		auto words = words_dict_create_from_raw(mem, mem_size);

		// Step 2: move elements to vector and sort them.
		auto sorted = words_dict_prepare_to_dump(std::move(words));

		// Step 3: dump sorted elements to output file.
		return words_holder_dump_to_file(sorted, output_fd);
	}
}; // namespace

int
main(int argc, char const *argv[])
{
	if (argc != ARGC_COUNT) {
		::fprintf(stderr, "Expected two command line arguments: input and output filenames, got %d\n", argc);
		return EX_USAGE;
	}

	PerfTimer timer{};
	timer.start();
	const char *input_filename  = argv[1];
	const char *output_filename = argv[2];

	if (words_process(input_filename, output_filename) != 0) {
		::fprintf(stderr, "Failed to count words :( \n");
		return EX_OSERR;
	}
	timer.stop();
	::fprintf(stdout, "Elapsed time: %f ms\n", timer.elapsed_ms());

	return EX_OK;
}
