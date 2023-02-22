# MD5

Takes an input string or file and outputs its MD5 hash.

This repo is gaining a little more traffic than I expected, so I'll put this here as a little disclaimer. I wrote this code as a side project in college in an attempt to better understand the algorithm. I consider this repository to be a reference implementation with a good step by step walkthrough of the algorithm, not necessarily code to be built upon. I did verify the correctness of the output by comparing to other existing standalone programs. However, I did not research edge cases, set up automated testing, or attempt to run the program on any machine other than the laptop I had at the time, so here's the warning:

This code may be generally correct, but you should consider it untested to be on the safe side. There may be edge cases, vulnerabilities, or optimizations I did not consider when I wrote this. I can only confirm that this code probably worked correctly on a single computer in 2017.

Knowing that, do feel free to use this code in any way you wish, no credit needed. And if you find a problem, raise an issue.

### Implementing into Code

If you want to include the md5 algorithm in your own code, you'll only need `md5.c` and `md5.h`.

```c
#include "md5.h"

...

void foo(){
    uint8_t result[16];
    md5String("Hello, World!", result);       // *result = 65a8e27d8879283831b664bd8b7f0ad4

    FILE bar = fopen("bar.txt", "r");
    md5File(bar, result);                     // Reads a file from a file pointer
    md5File(stdin, result);                   // Can easily read from stdin

    // Manual use
    ..
    MD5Context ctx;
    md5Init(&ctx);

    ...
    md5Update(&ctx, input1, input1_size);
    ...
    md5Update(&ctx, input2, input2_size);
    ...
    md5Update(&ctx, input3, input3_size);
    ...

    md5Finalize(&ctx);

    ctx.digest;                               // Result of hashing (as uint8_t* with 16 bytes)
}
```

### Command Line

You can directly use the binary built with this Makefile to process text or files in the command line.

Any arguments will be interpreted as strings. Each argument will be interpreted as a separate string to hash, and will be given its own output (in the order of input).

```shell
$ make

$ ./md5 "Hello, World!"
65a8e27d8879283831b664bd8b7f0ad4

$ ./md5 "Multiple" Strings
a0bf169f2539e893e00d7b1296bc4d8e
89be9433646f5939040a78971a5d103a

$ ./md5 ""
d41d8cd98f00b204e9800998ecf8427e

$ ./md5 "Can use \" escapes"
7bf94222f6dbcd25d6fa21d5985f5634
```
If no arguments are given, input is taken from standard input.
```shell
$ make

$ echo -n "Hello, World!" | ./md5
65a8e27d8879283831b664bd8b7f0ad4

$ echo "Hello, World!" | ./md5
bea8252ff4e80f41719ea13cdf007273

$ echo "File Input" > testFile | ./md5
d41d8cd98f00b204e9800998ecf8427e

$ cat testFile | ./md5
7dacda86e382b27c25a92f8f2f6a5cd8

```
As seen above, it is important to note that many programs will output a newline character after their output. This newline *will* affect the output of the MD5 algorithm. `echo` has the `-n` flag that prevents the output of said character.

If entering input by hand, end collection of data by entering an EOF character (`Ctrl+D` in some cases).

# The Algorithm

While researching this algorithm, the only relatively complete description I found came from RSA Data Security itself in [this memo][1]. And while the description is adequate, any confusion is very difficult to clear up, especially given the nature of the algorithm's output. So here I will try to describe the algorithm used in these implementations with examples.

The algorithm considers all words to be little-endian. I will also specify where this may be confusing.

The algorithm takes in an input of arbitrary length in bits. This can be a string, a file, a number, a struct, etc... It also doesn't need to be byte-aligned, though it almost always is. We'll call this input the message. The output is the digest.

#### Step 1: Padding

The provided message is padded by appending bits to the end until its length is congruent to `448 mod 512` bits. In other words, the message is padded so that its length is 64 bits less than the next multiple of 512. If the original message's length already meets this requirement before padding, it is still padded with 512 bits.

The padding is simply a single "1" bit at the end of the message followed by enough "0" bits to satisfy the length condition above.

##### Example

Let's pass the string "Hello, world!" to the algorithm. Those characters converted to hexadecimal numbers look like this:
```
48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21
```
(Note: Strings are often null-terminated. This null character is not taken into account, as you will see.)

Now we have to pad our message bits:
```
0x 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 80 00 00
0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x 00 00 00 00 00 00 00 00
```
Note the `0x80` right after the end of our message. We're writing a stream of bits, not bytes. Setting the bit after our message to "1" and the next 7 bits to "0" means writing the byte `1000 0000` or `0x80`.

#### Step 2: Appending the Length

Next, the length of the message modulus 2^64 is appended in little endian to the message to round out the total length to a multiple of 512. This length is the number of *bits* in the original message, modulus 2^64. It's common to split this number into two 32-bit words, so keep careful track of which bytes are put where; the highest order byte should be the last byte in the message. This will round out the length of the whole message to a multiple of 512.

##### Example 1

The length of our message is 104 bits. The 64-bit representation of the number 104 in hexadecimal is `0x00000000 00000068`. So we'll append that number to the end.

```
0x 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 80 00 00
0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x 00 00 00 00 00 00 00 00 68 00 00 00 00 00 00 00
```

(We're writing in little-endian, so the lowest order byte is written first.)

If you're holding the length in two separate 32-bit words, make sure to append the lower order bytes first.

##### Example 2

Because our "Hello, world!" example is so small and doesn't give a length with more than two digits, let's say we have a different, bigger message of `0x12345678 90ABCDEF` bits and this chunk we're looking at is just the tail end that we have to pad out. The appended length would look like this:

```
0x 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 80 00 00
0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x 00 00 00 00 00 00 00 00 EF CD AB 90 78 56 34 12
```

#### Step 3: Initializing the Buffer

The variables that will eventually hold our digest must be initialized to the following:

```
A = 0x01234567
B = 0x89abcdef
C = 0xfedcba98
D = 0x76543210
```

#### Step 4: Processing

There are four functions defined in the RSA memo that are used to collapse three 32-bit words into one 32-bit word:

```
F(X, Y, Z) = (X & Y) | (~X & Z)
G(X, Y, Z) = (X & Z) | (Y & ~Z)
H(X, Y, Z) = X ^ Y ^ Z
I(X, Y, Z) = Y ^ (X | ~Z)
```

These are bitwise operations.

We also have to do a left rotate on the bits in a word. That is, shift the bits left, and move overflow to the right. Like spinning a bottle and seeing the label loop around. The function is defined as follows:

```
rotate_left(x, n) = (x << n) | (x >> (32 - n))
```

The constants in K and S can be found at the bottom of this section.

The message is split into blocks of 512 bits. Each block is split into 16 32-bit words. For each block, do the following:

```c
AA = A;
BB = B;
CC = C;
DD = D;

for(i in 0 to 63){
    if(0 <= i <= 15){
        E = F(BB, CC, DD);
        j = i;
    }
    else if(16 <= i <= 31){
        E = G(BB, CC, DD);
        j = ((i * 5) + 1) % 16;
    }
    else if(32 <= i <= 47){
        E = H(BB, CC, DD);
        j = ((i * 3) + 5) % 16;
    }
    else{
        E = I(BB, CC, DD);
        j = (i * 7) % 16;
    }

    temp = DD;
    DD = CC;
    CC = BB;
    BB = BB + rotate_left(AA + E + K[i] + input[j], S[i]);
    AA = temp;
}

A += AA;
B += BB;
C += CC;
D += DD;
```

The RSA memo explicitly lists each step instead of using control structures. The result is the same.

An example for this step is not particularly useful, as the data produced by the loop is not very meaningful for observation.

#### Step 5: Output

The digest is a 128-bit number written in little endian, and is contained in A, B, C, and D after the algorithm is finished. Just arrange the bytes so that the lowest-order byte of the digest is the lowest-order byte of A, and the highest-order byte of the digest is the highest-order byte of D.

##### Example

Here is the output of a few strings to check against:

"Hello, world!"

```
6cd3556deb0da54bca060b4c39479839
```

"" (empty string)

```
d41d8cd98f00b204e9800998ecf8427e
```

"The quick brown fox jumps over the lazy dog."

```
e4d909c290d0fb1ca068ffaddf22cbd0
```

#### Constants and Functions

```c
A = 0x01234567
B = 0x89abcdef
C = 0xfedcba98
D = 0x76543210

K[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
       0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
       0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
       0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
       0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
       0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
       0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
       0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
       0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
       0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
       0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
       0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
       0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
       0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
       0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
       0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391}

S[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
       5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
       4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
       6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21}


F(X, Y, Z) = (X & Y) | (~X & Z)
G(X, Y, Z) = (X & Z) | (Y & ~Z)
H(X, Y, Z) = X ^ Y ^ Z
I(X, Y, Z) = Y ^ (X | ~Z)

rotate_left(x, n) = (x << n) | (x >> (32 - n))
```

[1]: https://tools.ietf.org/html/rfc1321
