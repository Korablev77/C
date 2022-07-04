#include <algorithm>
#include <vector>

#include "System.h"
#include "Words.h"

enum {
	KB = 1 << 10,
	MB = KB << 10
};

enum {
	// 2^32-1 == 4294967295 -- 10 characters
	UINT32_MAX_CHARS_COUNT = 10,
	WRITE_CHUNK_SIZE = 4 * MB
};

// Insert new value {word, 1} to a dictionary or increment already existing element.
static void
words_dict_count_frequency(WordsFreqDict &map, char *str, std::size_t len, std::size_t hash)
{
	LightStr word{str, len, hash};
	auto found = map.find(word);
	if (found != map.end()) {
		assert(found->second >= 1);
		//found->second++;
		// tsl::hopscotch_map returns const iterator to the element (not pair<const Key, T>);
		// so we have to explicitly refer to mutable reference.
		found.value()++;
	} else {
		map.insert({std::move(word), 1});
	}
}

// isalpha2() and tolower2() analogues of functions from libc.
static inline bool isalpha2(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static inline void tolower2(char &c)
{
	if (c >= 'A' && c <= 'Z') c += 32;
}

WordsFreqDict
words_dict_create_from_raw(char *raw_mem, std::size_t mem_size)
{
	assert(raw_mem != nullptr);
	WordsFreqDict words{};

	std::size_t word_len = 0;
	// Calculate hash in-place so that avoid second memory traversal.
	std::uint32_t hash = Hash::offset_basis;
	for (std::size_t offset = 0; offset < mem_size; ++offset) {
		if (! isalpha2(raw_mem[offset]) && word_len > 0) {
			assert(word_len <= offset);
			words_dict_count_frequency(words, raw_mem + offset - word_len, word_len, hash);
			word_len = 0;
			hash = Hash::offset_basis;
		}
		if (isalpha2(raw_mem[offset])) {
			tolower2(raw_mem[offset]);
			word_len++;
			hash ^= (unsigned char) raw_mem[offset];
			hash *= Hash::prime;
		}
	}
	return std::move(words);
}

/////////////////////////////////// Words Dump ////////////////////////////////////////////////////////////////////////

WordsDumpHolder
words_dict_prepare_to_dump(WordsFreqDict &&words)
{
	WordsDumpHolder sorted(std::make_move_iterator(words.begin()), std::make_move_iterator(words.end()));
	// Sort firstly by freqeunces and then by lexicographic order.
	std::sort(sorted.begin(), sorted.end(), [] (const WordCount &lhs, const WordCount &rhs) -> bool
		{
			if (lhs.second != rhs.second)
				return lhs.second >= rhs.second;
			return raw_bytes_cmp(lhs.first.ptr, lhs.first.len, rhs.first.ptr, rhs.first.len) < 0;
		});
	return std::move(sorted);
}

int
words_holder_dump_to_file(const WordsDumpHolder &sorted, int output_fd)
{
	assert(output_fd >= 0);
	static char _rwbuf[WRITE_CHUNK_SIZE];
	char *buf_pos = _rwbuf;
	char *buf_end = _rwbuf + WRITE_CHUNK_SIZE;
	for (const auto &w : sorted) {
		// To reduce the number of writes let's accumulate data in tmp buffer
		// and then (after reaching treshold) flush it to the disk. Don't bother with
		// mmap for output: a few writes seem to be enough here (see note concenring vector size).
		std::size_t entry_size = w.first.len + UINT32_MAX_CHARS_COUNT + 2;
		assert(entry_size < WRITE_CHUNK_SIZE && "Too large word: more than 4MB");
		if (buf_pos + entry_size >= buf_end) {
			if (file_write_chunk(output_fd, _rwbuf, buf_pos) < 0)
				return -1;
			buf_pos = _rwbuf;
		}
		assert(w.second > 0);
		buf_pos += snprintf(buf_pos, UINT32_MAX_CHARS_COUNT, "%lu", w.second);
		*buf_pos++ = ' ';
		assert(w.first.len > 0);
		my_memcpy(buf_pos, w.first.ptr, w.first.len);
		buf_pos += w.first.len;
		*buf_pos++ = '\n';
		assert(buf_pos < buf_end);
	}
	// Write the rest of the buffer. If buf_pos == _rwbuf returns immediately.
	if (file_write_chunk(output_fd, _rwbuf, buf_pos) < 0) {
		return -1;
	}
	// Make sure all data has reached output file. Not sure if it is
	// fair to account it in the benchmark, but let's do it.
	// if (::fsync(output_fd) != 0) {
	// 	::fprintf(stderr, "fsync() has failed: %s \n", ::strerror(errno));
	// 	return -1;
	// }
	return 0;
}
