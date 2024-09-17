# Lexicon

Lexicon is a C++ program that reads a text file and generates a dictionary of words and their frequencies for performance comparison purposes.

The program supports different types of dictionaries, including **AVL trees**, **Red-Black trees**, **Hash Tables with Chaining**, and **Hash Tables with Open Addressing**.

The program uses the **International Components for Unicode** (ICU) libraries to handle Unicode text and perform case-insensitive comparisons. The generated dictionary can be saved to an output file for further analysis.

## Installing ICU Libraries

To compile and run the code, you need to install the ICU libraries on your system. You can install the ICU libraries using the following command (for Ubuntu/Debian-based systems):

```bash
sudo apt-get install libicu-dev
```

This command will install the necessary ICU libraries on your system, allowing you to compile and run the code.

## Compiling

To compile your code using the g++ compiler and link against the ICU libraries, you can use the following command:

```bash
g++ main.cpp -o main -licuuc -licui18n
```

This command will compile the `main.cpp` file and generate an executable named `main`. The `-licuuc` and `-licui18n` flags are used to link against the ICU libraries.

Remember to make sure that the necessary ICU libraries are installed on your system before running this command.

## Execution

To execute the `main` executable, you need to provide two command-line arguments: the name of the input file and the type of dictionary. Here's an example of how to run the program with different dictionary types:

```bash
./main <file_name> <dictionary_type>
```

### Dictionary Types

1. AVL dictionary: `avl`

2. Red-Black dictionary: `rb`

3. Hash Table with Chaining dictionary: `htc`

4. Hash Table with Open Addressing dictionary: `hto`

### Input and Output Files

The input files should be placed in the data/in/ directory, and the program will generate output files in the data/out/ directory. Ensure that your input files are correctly located, and after execution, you can find the results in the data/out/ directory.

Make sure to replace `<file_name>` with the actual name of your input file, and choose the appropriate dictionary type.

Remember to install the necessary ICU libraries and compile the `main.cpp` file before running these commands.
