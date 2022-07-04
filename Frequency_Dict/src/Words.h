#pragma once

#include <tsl/hopscotch_map.h>

#include "LightStr.h"

// Hash table storing pairs of <string : frequency>; key is already lowercased.
using WordsFreqDict = tsl::hopscotch_map<LightStr, std::size_t, Hash::RawBytes, RawBytesEqual>;

WordsFreqDict
words_dict_create_from_raw(char *raw_mem, std::size_t mem_size);

/////////////////////////////////// Words Dump ////////////////////////////////////////////////////////////////////////

// Count of words shouldn't be too large: number of different words
// in English is 171,476. It's unlikely that in the text we meet
// more than several thousands. So the number of elements in map is
// not that large. Let's simply move the content of hash table to the
// vector and sort it in a straightforward way.

// WordCount is simply an element of @a WordsFreqDict container.
using WordCount = std::pair<LightStr, std::size_t>;
using WordsDumpHolder = std::vector<WordCount>;

// Construct vector of pairs<word : frequency> from HashTable containing
// <word : frequency> and sort the former. Return (move) resulting vector.
WordsDumpHolder
words_dict_prepare_to_dump(WordsFreqDict &&words);

int
words_holder_dump_to_file(const WordsDumpHolder &sorted, int output_fd);
