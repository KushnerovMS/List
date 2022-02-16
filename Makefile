CC=g++
include /home/kms/Cprojects/Cflags
LDFLAGS=
SOURCES=main.cpp FileWork.cpp RuCtype.cpp Sorting.cpp String.cpp UTF-8.cpp
OBJDIR =./.obj
OBJECTS=$(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
EXECUTABLE=Onegin
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
