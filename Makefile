RM=rm -r -d

SANFLAGS=-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CFLAGS=-D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations   \
	   -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts			\
	   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal 	\
	   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline 			\
	   -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked 				\
	   -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo 	\
	   -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn 					\
	   -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default 		\
	   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast 			\
	   -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing 	\
	   -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation 		\
	   -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer 		\
	   -fPIE $(SANFLAGS) -pie -Wstack-usage=8192

SOURCES=main.cpp List.cpp
OBJDIR =./.obj
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SOURCES))
EXECUTABLE=List
EXEDIR=./bin
LIBRARIES=kms/libLogs.a kms/libGraphDump.a

COMAPANY=kms

LIBDIR=/lib
HEADERDIR=/usr/include


$(EXECUTABLE): $(OBJECTS) $(EXEDIR)
	$(CXX) $(SANFLAGS) $(OBJECTS) -o $(EXEDIR)/$@ \
		$(addprefix -L$(LIBDIR)/, $(sort $(dir $(LIBRARIES)))) \
		$(patsubst lib%.a, -l%, $(notdir $(LIBRARIES)))


$(OBJECTS) : $(OBJDIR)/%.o : %.cpp
	mkdir -p $(@D)
	$(CXX) -c $(CFLAGS) $< -o $@ \
		$(addprefix -L$(LIBDIR)/, $(sort $(dir $(LIBRARIES)))) \
		$(patsubst lib%.a, -l%, $(notdir $(LIBRARIES)))

lib$(EXECUTABLE).a : $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	$(AR) r lib$(EXECUTABLE).a $(filter-out $(OBJDIR)/main.o, $(OBJECTS))


libInstall : lib$(EXECUTABLE).a $(LIBDIR)/$(COMAPANY)/ $(HEADERDIR)/$(COMAPANY)/
	sudo cp -f lib$(EXECUTABLE).a $(LIBDIR)/$(COMAPANY)/
	sudo cp -f $(EXECUTABLE).h 	 $(HEADERDIR)/$(COMAPANY)/

$(LIBDIR)/$(COMAPANY)/ :
	sudo mkdir $(LIBDIR)/$(COMAPANY)/

$(HEADERDIR)/$(COMAPANY)/ :
	sudo mkdir $(HEADERDIR)/$(COMAPANY)/


$(EXEDIR) :
	mkdir $(EXEDIR)

clear:
	$(RM) $(OBJDIR)

run : $(EXECUTABLE)
	$(EXEDIR)/$(EXECUTABLE)
