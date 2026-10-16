# otp-key-reuse-attack
C++ cryptanalysis of a many-time pad: recovers eight plaintexts encrypted with the same one-time pad key, using XOR of ciphertext pairs and space detection

This project introduces the basics of cryptanalysis through the one-time pad (OTP), and shows why reusing a key makes it dangerous.

A one-time pad is perfectly secure only when its key is random, as long as the message, and used **once**. Here, eight English sentences were all encrypted with the **same key**, and the program recovers the plaintexts without ever being given the key. It shows that reusing a key leaks the relationship between messages, and that the structure of English text is enough to turn that leak into the full plaintexts.

The plaintext is English letters and spaces only (some sentences end with padding spaces). Each ciphertext is 148 hex characters, which is 74 bytes.

## How it works

1. **Read the ciphertexts.** I read the file, printed each line's length (all 148 hex chars) and stored the lines in a `vector<string>`.

2. **XOR every pair.** For each pair of ciphertexts I XOR them byte by byte. The key cancels out, so `c1 ^ c2 = m1 ^ m2`. XOR is commutative, so each pair is done once, which is 28 pairs for 8 lines.

3. **Find possible spaces.** A space XOR a letter always has bit 6 set (`0x40`), and a letter XOR a letter never does. So if `(c1 ^ c2) & 0x40` is non-zero, one of the two characters at that position is a space. I store the position and the two cipher numbers in a vector of structs.

4. **Vote to decide which one is the space.** For each candidate I XOR both suspect ciphertexts with every other ciphertext at that position. The one that gives more letter-like results (bit 6 set) is the one holding the space. A tie is skipped.

5. **Get the key byte.** The plaintext there is a space, so the key byte is `cipher byte ^ 0x20`. Before saving it, I check that the byte decrypts all 8 ciphertexts to a letter or a space at that position. Without this check a wrong vote could overwrite a correct key byte, and the output had wrong characters at the end of the lines.

6. **Guess the rest.** Some key positions are never found because no sentence has a space there. I decrypt with the partial key and print `*` for the unknown bytes. Some words are easy to see, like `sys*** s*ft*are` for "system software". A helper function takes a guessed word, the ciphertext it appears in, and its start position, and works out the key bytes. Each guess reveals letters in all 8 messages, so the next one gets easier.

## Notes

- The program finds 44 of the 74 key bytes on its own. The rest come from guessing words.
