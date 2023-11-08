# Black
![Black](/res/logo.png "Black")
Stack based programming language made with C++17. Based on my previous project in Rust [Ktnack](https://github.com/Kirdow/Ktnack) which was based on [Tsoding/Porth](https://gitlab.com/tsoding/porth).

## Building & Running
You need CMake and GCC in order to build.<br>
Running would need an OS based on Unix/Linux which uses Linux syscalls.

Most of the compiling and running is done through `./run.sh`, you can edit it in order to check how it works.
But generally you'd run a file (e.g. `test.bk` which is provided) using `./run.sh <filename.bk>`.

You can also run tests found in `tests/` using `./test.sh`. This can be done by providing the test name,<br>
without the extension and directory. If you wanted to run `tests/00-push-log.bk`, run `./test.sh 00-push-log`.

## References
Inspired by [Porth](https://gitlab.com/tsoding/porth) by [Tsoding](https://www.youtube.com/@TsodingDaily).<br>
Also based upon a project I made in March 2023 called [Ktnack](https://github.com/Kirdow/Ktnack) which was written in Rust,<br>
and Ktnack was also inspired by [Porth](https://gitlab.com/tsoding/porth).

# Feature set
Black has a wide feature set which share similarities between Ktnack and Porth. Here's a general guide on how to use each feature.

## Push
In order to push a value to the stack, you simply type the value out, and it's pushed to the stack.
```
12
```
This would be identical to typing `push 12` in x86-64 assembly.

## Log
Log logs the current value on top of the stack, as well as consuming it. It's used by typing `.` and works as follows:
```
12 .
```
This will first push `12` on top of the stack.<br>
Then we use `.` to pop the top value (12) and then print it to the screen.<br>
Take note that all integer values used are 64-bit (8 bytes) in size.

## Arithmetic operators
Arithmetics is fairly straight forward. As with everything stack based, you'd often see `<value 1> <value 2> <operand>` rather than <value 1> <operand> <value 2>`.<br>
Here's how you'd use `+`, `-`, `*`, `/` and `%`:
```
7 5 +
7 5 -
7 5 *
7 5 /
7 5 %
```

## Logical operators
Logical operators work similar to arithmetic operators but instead use `<`, `>`, `<=`, `>=`, `=` and `!=`.
They take the top 2 items on the stack, and compare them. If the comparison is successful, they push `1` onto the stack,
otherwise they push `0` onto the stack.
```
7 5 < .
7 5 > .
```
From the above code, the output would be:
```
0
1
```

## If statement
If statement pops the top value from the stack, and choose the branch depending on the value.

If the value is non-zero, it enters the body, otherwise, it jumps to the `else` body if it exists, otherwise to the `end` body.

**Here are a few examples and their outputs**
Code:
```
1 if
    1 .
end
0 if
    2 .
end
```
Output:
```
1
```

Code:
```
1 if
    1 .
else
    2 .
end
```
Output:
```
1
```

Code:
```
0 if
    1 .
else
    2 .
end
```
Output:
```
2
```

For a more dynamic example you could use logical operators
```
5 7 < if
    1 .
end
```
This would output `1`.

If statements also suppport nesting.

## Dup, over, swap and drop
These are 4 operators which manipulate the stack directly.

### Dup
`dup` will pop the top value, and push it back twice
```
5 7 dup
. . .
```
Output:
```
7 7 5
```

### Over
`over` takes the second value on the stack and copies it to the top
```
5 7 over
. . .
```
Output:
```
5 7 5
```

### Swap
`swap` takes the top two values on the stack and switch their places
```
5 7 swap
. .
```
Output:
```
5 7
```

### Drop
`drop` will pop the value off the stack and discard it.<br>
It's used when you no longer need a value, in order to clean up the stack.<br>
This is used a lot with loops.
```
5 7 drop
.
```
Output:
```
5
```

## While loop
While loops start with `while`, followed by the code which acts as the condition.<br>
Being stack based, there's no limit on the size to this. This section ends with `do`<br>
which is where the loop's body starts. The loop itself ends with `end`, which will<br>
jump back to `while`, where the condition will be checked.

The condition works exactly like `if`, where if the value is `0` it jumps past `end`,<br>
otherwise it runs the loop's body.

Here's an example of a loop iterating from 0 through 9:
```
0 while dup 10 < do
    dup .
    1 +
end
drop
```
This first sets the starting point to `0`.<br>
Now we make the condition `is the value less than 10`.<br>
Since we consume the values with any operation, we need to duplicate it,<br>
which is where `dup` comes in. Then inside the loop, we once again<br>
duplicate the value in order to print it. Then we add 1 to the original value.<br>
Lastly after the loop we use `drop` as we won't need the value anymore.<br>
Without the use of `drop` the stack would be misaligned, so we drop it.

## Memory access
You also get access to a buffer of 640,000 bytes.<br>
This is accessed using two functions, `S` for save and `L` for load.<br>
These allow you to save single bytes at various locations.<br>

You get the buffer address using `@`, and to get an exact address<br>
within it, for example index `5`, you'd do `@ 5 +`.

Save takes `value address S`.
```
12 @ 6 + S
```
This saves the value `12` at index 6 in the buffer.

Load takes `address L`.
```
@ 3 + L
```
This loads from index `3` in the buffer and puts the value on the stack.

## Printing strings
Accessing the memory allows you to push utf8 values onto the memory,<br>
which you can then print using `P` or `p`.<br>
`p` simply takes the buffer at the address and prints them to `stdout`.<br>
`P` works the same as `p` but also adds a new line at the end.

Printing takes `address count P`.
```
65 @ 0 + S
66 @ 1 + S
67 @ 2 + S
@ 0 + 3 P
```
This prints `ABC` with a new line.

## String literals
String literals are strings defined in the source code using quotes.<br>
Using string literals pushes its address and character count onto the stack,<br>
n a way where it's ready to print.

String literals are located at its own piece of the executable just like with<br>
oher compiled languages, and should thus not be written to.
*This would in x86-64 assembly be the `segment .data` block.*

You can define and print a string literal like this:
```
"Hello, World!" P
```
*Woo finally hello world!*<br>
This prints what you'd expect.

## Exit Code
By default the program already has a value of `0` on top of the stack. This means
that if nothing is written in the program, the program exists with exit code 0.

The exit code is determined by taking the value on top of the stack, and passing it as `rdi` to syscall 60 (exit syscall).

As you may notice, the tests written in `tests/` use this to determine if the tests are successful or not.
