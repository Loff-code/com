#!/bin/bash

# Default input file
default_file="risc.s"

# Check if an input file is provided, otherwise use the default
if [ $# -eq 0 ]; then
    echo "No input file provided. Using default file: $default_file"
    input_file="$default_file"
else
    input_file="$1"
fi

# Ensure the input file ends with .s
if [[ "$input_file" != *.s ]]; then
    echo "Error: Input file must be a .s file."
    exit 1
fi

# Derived filenames
object_file="${input_file%.s}.o"
output_file="machine_code.txt"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' not found."
    exit 1
fi

# Assemble the .s file into an .o file
riscv64-unknown-elf-as -o "$object_file" "$input_file"
if [ $? -ne 0 ]; then
    echo "Error: Failed to assemble '$input_file'."
    exit 1
fi

# Extract machine code, prepend 0x, add a comma, and save to output file
riscv64-unknown-elf-objdump -d "$object_file" | \
grep '^[[:space:]]*[0-9a-f]*:' | \
awk '{print "0x" $2 ","}' > "$output_file"

# Remove the intermediate .o file
rm "$object_file"

# Notify the user
echo "Machine code written to '$output_file' with 0x prefix and one instruction per line. Intermediate object file removed."
