# MD5 (JavaScript)
A function for providing the MD5 hash of a string.

JavaScript is an awful language to write this kind of program in. But, the algorithm is useful. This program is not very well-optimized, and it will always return a string instead of a number (for the safety of floating point imprecision). It also always calls the toString method of whatever input it is given. The file can be adapted for either client-side scripts or server-side programs, and works as either in its current form.
