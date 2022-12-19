
INCLUDEDIR=headers 
C=gcc
CFLAGS=
LDFLAGS=
SOURCES=src/lexical_analyzer.c src/name_table.c
OBJDIR=build
OBJECTS=$(SOURCES:%.c=$(OBJDIR)/%.o)
EXECUTABLE=interpreter

all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(C) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(@D)
	$(C) -c $(CFLAGS) -I$(INCLUDEDIR) $< -o $@

clean:
	rm -f interpreter
	rm -rf build