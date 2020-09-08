# Notes about the Challenges

This doc contains my notes related to the challenges.

## Set 1

### Challenge 1

It's helpful to deal with a fundamental bytes type and let a string type
represent an encoding of bytes. This way, there won't be problems with guessing
encodings, and I can control the conversion between bytes and strings.

To represent bytes, I chose to use the type `std::vector<uint8_t>`. The class
`cryptopals::util::Bytes` encapsulates this byte array and contains useful
functions for converting to/from strings.

To avoid encoding pitfalls, I will always construct bytes using a factory
function that takes a `string_view` as input. I support the following formats:
`hex`, `base64`, and `raw` (interpret the bytes of the string literally).

```sh
./build/src/cryptopals/challenges/01/convert_tool --from hex --to base64 \
49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
```

### Challenge 2

While it says fixed xor, I found it much more helpful to implement what I'll
call repeating xor. The right operand will be repeated or truncated in order to
match the length of the left operand before performing the xor. Here's an
example:

```txt
# The right operand (abc) is much shorter than the left operand.
    1111 1111 2222 2222 3333 3333
XOR abc

# Repeat the right operand until it matches the length of the left operand.
    1111 1111 2222 2222 3333 3333
XOR abca bcab cabc abca bcab cabc
  = badb adba e89e 89e8 8f98 f98f
```

Because I have a `Bytes` class, it naturally makes sense to overload `operator^`
and `operator^=` to implement this.

```sh
./build/src/cryptopals/challenges/01/xor_tool --format hex 1234 ABCD
```

### Challenge 3

Lots of infrastructure is required for this.

To begin, I want to define a `CipherInterface` type which provides encrypt and
decrypt methods. I tried several designs to store a key, but since the key
material could be different based on the cipher, it would need to be a template
variable. Also, since `CipherInterface` is an abstract base class, I needed to
be careful with templates and polymorphism. Therefore, I ended up making the key
a parameter to the functions.

The `SingleByteXor` cipher uses a `uint8_t` as a key type. In order to reuse the
xor code from challenge 2, I added the factory function `CreateFromIntegral` to
the `Bytes` class. The encrypt and decrypt functions use the xor of two bytes
objects.

Next I need to guess the key. To do this, I can brute force each of the
potential keys and then chose the most likely output with frequency analysis. I
created an `AnalyzerInterface` which will analyze some bytes and produce a score
indicating the likelihood of those bytes being the correct decryption. A higher
score is a more likely decryption.

The `EnglishAsciiFrequencyAnalyzer` will score decrypted text based on the
frequency of letters in the English language (note that each byte is expected to
be encoded in ASCII). My scoring function is as follows: calculate the frequency
of each letter in the observed text, and sum the differences between the
observed frequency and the expected frequency of each letter (data obtained from
Wikipedia). Subtract the error from 100, and add a penalty for non-printable
ASCII characters. In the future, I would like to use a chi-squared test or
another more formal statistical measure.

I can put this all together in the `single_byte_xor_tool` to crack ciphertext
that was encoded with this format.

```sh
./build/src/cryptopals/challenges/01/single_byte_xor_tool --action encrypt --format hex --key AA \
0123456789ABCDEFEDCBA9876543210 --logtostderr
```

### Challenge 4

Just use the `single_byte_xor_tool` against each string. I found that the actual
solution had a score of `19.732` in my scoring system.

However, I noticed that my scoring system was brittle to non letter characters,
which are still common in ASCII. To solve this, I created a reference frequency
table from `OANC` data. I created a `frequency_modeler` tool which will count
the bytes in a series of input files and print out the relative frequency of
each byte. I saved that in the `oanc_english` data to use in the
`AnalyzerInterface`.

I also updated the `frequency_analyzer` to plug in frequency data and calculate
a chi-squared statistic against analyzed bytes to determine how closely they
match the frequency data. With this more rigorous system, I can now handle all
common ASCII characters in American English.

```sh
./build/src/cryptopals/tools/frequency_modeler --filemap oanc_files.txt | sort -n
# Manually save output in oanc_english.h
xargs -n 1 ./build/src/cryptopals/challenges/01/single_byte_xor_tool --action crack --format hex \
< src/cryptopals/challenges/01/data/4.txt
```

### Challenge 5

Because of the modified xor implemented in challenge 2, this challenge was also
a breeze. I implemented the `RepeatingKeyXor` cipher with a `Bytes` object as
the key.

I did have a bit of trouble parsing the input correctly (since I use the command
line to pass the input). I ended up opening a quoted string, then I manually
entered a newline (no escaped `\n`), and finished the string.

```sh
./build/src/cryptopals/challenges/01/repeating_key_xor_tool --action encrypt --key 494345 --format hex --logtostderr \
"Burning em, if you ain't quick and nimble \nI go crazy when I hear a cymbal"
```

### Challenge 6

Breaking the `RepeatingKeyXor` cipher was a little bit more involved. I began by
implementing the Hamming Distance Analyzer, which involved adding a
`CompareBytes` method to the `AnalyzerInterface` class. The modern C++ standard
library provides functions to make this easy; I ended up using
`std::inner_product` and `std::bitset::count` to implement this.

The next step was to try to guess the key length from the ciphertext. I looped
over the key lengths from 2 to 40 and split the ciphertext into chunks of the
possible key length. I calculated the hamming distance between each pair of
chunks, took the average, and divided it by the key length. It turns out that my
specific implementation provided the correct key with the lowest score; however
this is susceptible to noise, so I used the 4 most likely results in my next
steps.

For each possible key length `n`, I split the ciphertext into segments
containing every `nth` byte and decrypted each individual segment using the
single-byte XOR cipher. After recombining the decrypted text, I used the
`frequency_analyzer` to ensure I retained the best solution for each tested key
length.

```sh
./build/src/cryptopals/challenges/01/repeating_key_xor_tool --action crack --format base64 \
--input ciphertext_file src/cryptopals/challenges/01/data/6.txt --logtostderr
```

### Challenge 7

Implementing the decryption of AES is nice; but for full understanding of what
I'm doing, I decided to implement encryption of AES first. There's a lot of math
behind the encryption standard that I'd like to explore more in detail
(especially around the choice of the SBox), but for algorithmic efficiency, I
chose to use a lookup table.

Most of the implementation work is done in `util/aes.cpp`; I pretty much
followed the spec and implemented each function as it was described. The
[AES spec][aes-spec] and the [AES validation program][aes-validate] were the
most helpful for implementing the functions and setting up test cases.

[aes-spec]: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
[aes-validate]: https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes/AESAVS.pdf

Annoyingly, they gave the key in ASCII which needed to be converted to base64
first. Good thing I have the convert_tool still!

```sh
./build/src/cryptopals/challenges/01/convert_tool --from raw --to base64 "YELLOW SUBMARINE"
./build/src/cryptopals/challenges/01/aes_ecb_tool --action DECRYPT --format base64 \
--key WUVMTE9XIFNVQk1BUklORQ== --input ciphertext_file src/cryptopals/challenges/01/data/7.txt
```

### Challenge 8

It is possible to detect AES in ECB mode because under the same key, the same
plaintext block always maps to the same ciphertext block. We can use this fact
to detect if there are any repeating patterns in the plaintext block.

I created the `aes_block_analyzer` to output the number of blocks that have the
same output. By running this analyzer against each of the encoded texts, it's
possible to determine which one is most likely to have been encoded using AES in
ECB mode.

At this point, I'm handling multiple forms of input (stdin, a single file
containing newline-separated ciphertext, and a file containing multiple
ciphertexts separated by a newline). I decided to create tool helper functions
to process these inputs and process enumerations provided at the command line.
The result of this is that the old command lines are broken; I went ahead and
updated the commands in the previous challenges to reflect the new arguments.

```sh
./build/src/cryptopals/challenges/01/aes_ecb_tool --action detect --format hex \
--input multi_ciphertext_file ./src/cryptopals/challenges/01/data/8.txt
```

## Set 2

### Challenge 9

Implementing PKCS#7 padding is straightforward. The number of padding bytes, and
the value of those padding bytes is `block_size - (input_size % block_size)`.
This will result in a full block of padding if the input is a multiple of the
block size. Also, using `uint8_t` as the type for block size ensures that we
don't request too many bytes of padding.

To remove the padding, I used `std::all_of` with a lambda expression to check
that the bytes input matches the expected value. With a few unit tests, I got
this code up and running.
