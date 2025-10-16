
all:
	@echo "Choice the option"

1:
	gcc 1_errors.c -o 1 -g
	mkdir -p tmp
	touch tmp/file1.txt
	./1 tmp/non_existing_file.txt
	./1 tmp/file1.txt

2_3:
	gcc 2_3_rdonly_file.c -o 2_3 -g
	mkdir -p tmp
	./2_3 tmp/file2_3.txt 644

4_5:
	gcc 4_5_rdwr_file.c -o 4_5 -g
	mkdir -p tmp
	./4_5 tmp/file4_5.txt 644

6:
	gcc 6_sparsed_file.c -o 6 -g
	mkdir -p tmp
	./6 tmp/file6.txt 644
	du -h tmp/file6.txt

7:
	gcc 7_stat.c -o 7 -g
	mkdir -p tmp
	./7 tmp/file6.txt

8:
	gcc 8_cp.c -o 8 -g
	mkdir -p tmp
	./8 6_sparsed_file.c tmp/file8.txt
	./8

9:
	gcc 9_cp.c -o 9 -g
	mkdir -p tmp
	./9 6_sparsed_file.c tmp/file9.txt
	./9

10:
	gcc 10_reverse_output.c -o 10 -g
	mkdir -p tmp
	echo 'Hello\!\!' > tmp/file.txt 
	./10 tmp/file.txt

clean:
	rm -r 1 2_3 4_5 6 7 8 9 10 tmp