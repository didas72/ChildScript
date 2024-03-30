# Child Script - Clean documentation

This is the 'clean' version of the documentation for Child Script. Here all jokes, context and justifications are removed for clarity. Some terms were also changed to improve readability. These are listed below.

## Terminology

To help readability, some terms used in the 'joke' documentation were replaced with more usable ones.

Joke term | Technical term | Notes
-- | -- | --
Joe / five-year-old | Intepreter | Interpreter may also refer to the entire interpreter state (including registers and memory contents)
Joe's mom | Input | Will usually also translate to stdin.
Joe's dad | Output | Will usually also translate to stdout.
Boxes | Registers / Memory | Colored boxes translate to registers, while numberes ones translate to memory.
Red box | `R` / Accumulator | -
Green box | `G` / Aux G | -
Blue box | `B` / Aux B | -
Yellow box | `Y` / Address | -
Color label | Label | Name shortened for convenience

The following translations are provided for completeness sake but are rarely, if ever, used.

- Plastic balls = Units
- Ball park = Source / Sink, depending on the usage.

## Source files

Child Script source files are expected to be ASCII text files. Other text encodings are not supported.

The default file extension for Child Script source files is `.csh`. The `.cs` extension avoided to prevent confusion with `C#` source files, even though the two languages can be easily distinguished.

## Registers and memory

The interpreter implements four registers and 256 cells of memory, all of which are internally kept and used as unsigned 8-bit integers.

Arithmetic operations wrap on overflow but do **NOT** wrap on underflow, instead the result is clamped to 0.

Both registers and memory are initialized to zero before execution.

While being, in some ways, general purpose, the four registers have intended uses:

- `R` is an accumulator. All arithmetic operations are performed on this register.
- `Y` is an address register. It is used as the address for every operation involving memory.
- `G` and `B` are general purpose.

## Commands

Currently, the following commands are defined:

Command | Meaning | Implemented
-- | -- | --
`X <COLOR>` | Boxes\[COLOR\] = 0 | V
`V O^<X>` | Boxes\[RED\] = (Boxes\[RED\] + X) & 0xFF | V
`A O^<X>` | If (Boxes\[RED\] > X) then<br>Boxes\[RED\] = Boxes\[RED\] - X<br>else<br>Boxes\[RED\] = 0<br>endif | V
`<-> <COLOR>` | T = Boxes\[RED\]<br>Boxes\[RED\] = Boxes\[COLOR\]<br>Boxes\[COLOR\] = T | V
`<=> <COLOR>` | T = Boxes\[COLOR\]<br>Boxes\[COLOR\] = Boxes\[Boxes\[YELLOW\]\]<br>Boxes\[Boxes\[YELLOW\]\] = T | V
`A>V <COLOR>` | If (COLOR not equals RED) then<br>Boxes\[RED\] = (Boxes\[RED\] + Boxes\[COLOR\]) & 0xFF<br>Boxes\[COLOR\] = 0<br>endif | V
`= <COLOR>` | Boxes\[COLOR\] = Boxes\[RED\] | V
`mama` | Boxes\[RED\] = input_ASCII() | V
`papa` | output_ASCII(Boxes\[RED\]) | V
`LABEL` | Defines a label | V
`O? LABEL` | If (Boxes\[RED\] equals 0) then<br>goto LABEL<br>endif | X

Lines starting with `#` or `//` are ignored and may be used for comments. Empty lines, are also ignored.
