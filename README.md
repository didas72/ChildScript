# Child Script

`Child Script` is my first (and hopefully only) esoteric language.  
The core idea behind Child Script is to create a language so simple that even a five-year-old could interpret it. To prove this point, the language interpreter simulates a kid (named Joe) who performs the tasks described in a script.

Child Script is intended to have a fairly comedic tone, so the documentation is usually cluttered with little jokes or context to justify the decisions. Since not everyone will be interested in that side, there will be two versions of the documentation: a cluttered one and a clean one. The latter will be stripped of all the context, providing more technical information.

- The funny version of the docs is available [here](https://esolangs.org/wiki/Child_Script).
- The [technical version](doc/clean_docs.md) will be kept in this repository, under the `doc` directory.

If the documentation happens to have discrepancies between the two version or describes behaviour different from what is observed in the interpreter, the interpreter should be used as the reference point (except if the interpreter is clearly buggy, e.g., a crash with a specific input).

## Examples

This language, like a lot of esoteric languages, is more easily understood by looking at [examples](examples).

## Compilation

In order to compile the interpreter, you will need to have `gcc` installed.
To compile the interpreter, simply clone the repository to a location of your choosing and run `make`. The produced binary will be put in `build/bin`.
You may choose to run the provided `hello.chs` test script with `make run` or call the interpreter with a different source file.
