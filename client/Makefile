CXX := gcc
LD := gcc
LDFLAGS := -lpthread -D_REENTRANT -g -lcsfml-audio -lcsfml-graphics -lcsfml-network -lcsfml-system -lcsfml-window 
SOURCES := src
INCLUDE := include
OBJECTS := obj
EXECUTABLE := client

CFLAGS := -I $(INCLUDE)
SOURCE := $(foreach dir, $(SOURCES), $(notdir $(wildcard $(dir)/*.c)))
OBJECT := $(SOURCE:.c=.o)

all: clean $(EXECUTABLE)

$(EXECUTABLE): $(addprefix $(OBJECTS)/, $(OBJECT))
	$(LD) $(LDFLAGS) -o $@ $^
	
$(OBJECTS)/%.o: $(SOURCES)/%.c
	$(CXX) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -rf obj/*.o $(EXECUTABLE)

