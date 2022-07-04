# Extremely primitive and inefficient implementation.

def word_count(input_filename, output_filename):
    input_file = open(input_filename, "r")
    output_file = open(output_filename, "w")
    output_file.truncate()

    chunk_size = 4096

    from collections import Counter

    words = []
    buf = ""
    buf = input_file.read(chunk_size)
    while buf != "":
        buf.lower()
        word = ""
        for letter in buf:
            if letter.isalpha():
                word += letter
            elif word != "":
                    words.append(word)
                    word = ""
        buf = input_file.read(chunk_size)

    counter = Counter(words)
    # This is not even correct since words will be sorted in reverse order.
    # But for benchmark sake it doesn't really matter...
    counter = sorted(counter.items(), key = lambda item : (item[1], item[0]), reverse = True)
    for key, value in counter:
        output_file.write(key + " : " + str(value)+"\n")
    input_file.close()
    output_file.close()

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Frequency dictionary ')
    parser.add_argument('input', nargs = 1, type = str)
    parser.add_argument('output', nargs = 1, type = str)
    args = parser.parse_args()

    import time
    start = time.perf_counter()
    word_count(args.input[0], args.output[0])
    stop = time.perf_counter()
    print("Elapsed time:", round((stop - start) * 1000), "ms")

