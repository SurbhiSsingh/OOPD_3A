# Default target: build all
all: quest1 quest2

# Target for quest1 (debug and optimized versions)
quest1: quest1.o
	g++ -g -o debug1 quest1.o          # Debug build of quest1
	g++ -O3 -o optimize1 quest1.o     # Optimized build of quest1
	./debug1                           # Run the debug version of quest1

# Compile quest1.o from quest1.cpp
quest1.o: quest1.cpp
	g++ -c quest1.cpp                  # Compile quest1.cpp to object file

# Target for quest2 (debug and optimized versions)
quest2: quest2.o
	g++ -g -o debug2 quest2.o          # Debug build of quest2
	g++ -O3 -o optimize2 quest2.o     # Optimized build of quest2
	./debug2                           # Run the debug version of quest2

# Compile quest2.o from quest2.cpp
quest2.o: quest2.cpp
	g++ -c quest2.cpp                  # Compile quest2.cpp to object file

# Clean up the object files and executables
clean:
	rm -f quest1 quest2 quest1.o quest2.o debug1 debug2 optimize1 optimize2

# Rebuild everything from scratch (clean + all)
rebuild: clean all
