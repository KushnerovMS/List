CC=g++
CFLAGS=-Wall
LDFLAGS=
SOURCES=main.cpp List.cpp	
OBJDIR =./.obj
OBJECTS=$(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
EXECUTABLE=List
EXEDIR=./bin


$(EXECUTABLE): $(OBJECTS) $(EXEDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXEDIR)/$@

$(OBJECTS) : $(OBJDIR)/%.o : %.cpp $(OBJDIR) 
	$(CC) -c $(CFLAGS) $< -o $@


lib$(EXECUTABLE).a : $(OBJECTS:$(OBJDIR)/main.o= )
	ar r lib$(EXECUTABLE).a $(OBJECTS:$(OBJDIR)/main.o= )

$(EXEDIR) :
	mkdir $(EXEDIR)

$(OBJDIR) :
	mkdir $(OBJDIR)
