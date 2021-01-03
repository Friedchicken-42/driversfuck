# A brainfuck compiler with drivers

## THIS IS USELESS, REALLY
### you shouldn't use this.


How drivers work:
```
each driver occupies a part of the tape, starting from position 0

so the first N cells are reserved to communicate with the drivers
```

with `.` and `,`, instead of interacting with the user, the tape interacts with the driver

Every driver must have at least 3 functions:\
init(void)       -> int\
input(char, int) -> void\
output(int)      -> char

`init` is used as initializes, return a number of cells used by the driver\
`input` saves a value from the tape to the driver\
`output` loads a value from the driver to the tape

## Compile
`make dll name=driver_name` to compile a driver
or `make` for everything


## Example - Hello World!
~~~
init initializes the length of the message and return 2;
    2 cells are reserved from the tape
input is not used
output change due to the position
    with pos == 0 returns how many characters until the end
    with pos == 1 returns current character and move the pointer to the next location
~~~
and can print `Hello World!` with this instructons: `+[,>,[>+<-]>.[-]<<]`\
_compared to a normal Hello World!: `++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.`_

the code loop through every character until there are 0 char left in the buffer, copy that value in a "normal" cell and output to video

## TO DO
- [x] add unix support
- [ ] add more drivers (file i/o, networking, ...)
