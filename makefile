
newmain: newmain.o macro.o transition1.o read_data.o opcode.o second_pass.o create_files.o free_file.o
	gcc -g -ansi -pedantic -Wall  newmain.o macro.o transition1.o read_data.o opcode.o second_pass.o create_files.o free_file.o -o assembler
newmain.o: newmain.c macro.h transition1.h
	gcc -c -ansi -pedantic -Wall newmain.c -o newmain.o
macro.o: macro.c macro.h
	gcc -c -ansi -pedantic -Wall macro.c -o macro.o
transition1.o: transition1.c transition1.h
	gcc -c -ansi -pedantic -Wall transition1.c -o transition1.o
read_data.o: read_data.c transition1.h
	gcc -c -ansi -pedantic -Wall read_data.c -o read_data.o
opcode.o: opcode.c transition1.h
	gcc -c -ansi -pedantic -Wall opcode.c -o opcode.o
second_pass.o: second_pass.c transition1.h
	gcc -c -ansi -pedantic -Wall second_pass.c -o second_pass.o
create_files: create_files.c transition1.h
	gcc -c -ansi -pedantic -Wall create_files.c -o create_files.o
free_file.o: free_file.c transition1.h
	gcc -c -ansi -pedantic -Wall free_file.c -o free_file.o
