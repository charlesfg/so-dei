# Makefiles are a simple way to manage the build process for a software project.
# They contain rules that specify how to generate target files from source files.
# The 'make' command reads the Makefile to determine what actions need to be taken.

# The Makefile consists of "targets", "prerequisites", and "recipes":
# - A target is usually the file that is generated by a program.
# - Prerequisites are files that are used as input to create a target.
# - A recipe is an action that 'make' takes to build a target from the prerequisites.

# Comments start with a # and go to the end of the line.

# Here we define variables that we can use later in the Makefile.
# Variables allow for more readable and maintainable Makefiles.

# The 'CC' variable holds the name of the C compiler we want to use.
CC=gcc

# The 'CFLAGS' variable holds compiler flags that we want to use.
# -Wall tells the compiler to give verbose warnings.
# -g tells the compiler to include debugging information in the executable.
CFLAGS=-Wall -g

# The 'TARGET' variable is the name of the final executable file we want to create.
TARGET=your_program

# 'all' is a conventional target that builds everything that needs to be built.
# Here we define it to depend on the $(TARGET), which means it will build the TARGET.
all: $(TARGET)

# This rule says how to build the target 'your_program' from 'your_program.c'.
# The target is on the left side of the colon, and the prerequisites are on the right side.
$(TARGET): your_program.c
	# The recipe part is below. It starts with a tab (not spaces).
	# '$(CC)' is replaced by the value of the CC variable, which is 'gcc'.
	# '$(CFLAGS)' is replaced by the value of the CFLAGS variable.
	# The '-o $@' tells the compiler to output the result to the file name of the target.
	# '$^' is a special variable that means "all the prerequisites", in this case 'your_program.c'.
	# '-lm' tells the compiler to link against the math library.
	$(CC) $(CFLAGS) -o $@ $^ -lm

# The 'clean' target is used to remove files that were generated during the build.
# It's not a file itself, so we mark it as .PHONY, which means it's "phony" or not a real file target.
clean:
	# Remove the TARGET file and any file ending in a tilde (~), which are backup files.
	rm -f $(TARGET) *~

# 'run' is a convenience target to compile, if necessary, and run the program.
run: $(TARGET)
	./$(TARGET)

# '.PHONY' tells make that 'all', 'clean', and 'run' are not files, even if there are files named 'all', 'clean', or 'run'.
.PHONY: all clean run
