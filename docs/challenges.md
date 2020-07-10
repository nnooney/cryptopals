# Notes about the Challenges

This doc contains my notes related to the challenges.

## Set 1

### Challenge 1

It's helpful to deal with a fundamental bytes type and let a string type represent an encoding of bytes. This way, there won't be problems with guessing encodings, and I can control the conversion between bytes and strings.

To represent bytes, I chose to use the type `std::vector<uint8_t>`. The class `cryptopals::util::Bytes` encapsulates this byte array and contains useful functions for converting to/from strings.

To avoid encoding pitfalls, I will always construct bytes using a factory function that takes a `string_view` as input. I support the following formats: `hex`, `base64`, and `raw` (interpret the bytes of the string literally).

### Challenge 2

While it says fixed xor, I found it much more helpful to implement what I'll call repeating xor. The right operand will be repeated or truncated in order to match the length of the left operand before performing the xor. Here's an example:

```txt
# The right operand (abc) is much shorter than the left operand.
    1111 1111 2222 2222 3333 3333
XOR abc

# Repeat the right operand until it matches the length of the left operand.
    1111 1111 2222 2222 3333 3333
XOR abca bcab cabc abca bcab cabc
  = badb adba e89e 89e8 8f98 f98f
```

Because I have a `Bytes` class, it naturally makes sense to overload `operator^` and `operator^=` to implement this.

### Challenge 3

Lots of infrastructure is required for this.

To begin, I want to define a `CipherInterface` type which provides encrypt and decrypt methods. I tried several designs to store a key, but since the key material could be different based on the cipher, it would need to be a template variable. Also, since `CipherInterface` is an abstract base class, I needed to be careful with templates and polymorphism. Therefore, I ended up making the key a parameter to the functions.

The `SingleByteXor` cipher uses a `uint8_t` as a key type. In order to reuse the xor code from challenge 2, I added the factory function `CreateFromIntegral` to the `Bytes` class. The encrypt and decrypt functions use the xor of two bytes objects.

Next I need to guess the key. To do this, I can brute force each of the potential keys and then chose the most likely output with frequency analysis. I created an `AnalyzerInterface` which will analyze some bytes and produce a score indicating the likelihood of those bytes being the correct decryption. A higher score is a more likely decryption.

The `EnglishAsciiFrequencyAnalyzer` will score decrypted text based on the frequency of letters in the English language (note that each byte is expected to be encoded in ASCII). My scoring function is as follows: calculate the frequency of each letter in the observed text, and sum the differences between the observed frequency and the expected frequency of each letter (data obtained from Wikipedia). Subtract the error from 100, and add a penalty for non-printable ASCII characters. In the future, I would like to use a chi-squared test or another more formal statistical measure.

I can put this all together in the `single_byte_xor_tool` to crack ciphertext that was encoded with this format.

### Challenge 4

Just use the `single_byte_xor_tool` against each string. I found that the actual solution had a score of `19.732` in my scoring system.

### Challenge 5

Because of the modified xor implemented in challenge 2, this challenge was also a breeze. I implemented the `RepeatingKeyXor` cipher with a `Bytes` object as the key.

I did have a bit of trouble parsing the input correctly (since I use the command line to pass the input). I ended up opening a quoted string, then I manually entered a newline (no escaped `\n`), and finished the string.

### Challenge 6

Breaking the `RepeatingKeyXor` cipher was a little bit more involved. I began by implementing the Hamming Distance Analyzer, which involved adding a `CompareBytes` method to the `AnalyzerInterface` class.
