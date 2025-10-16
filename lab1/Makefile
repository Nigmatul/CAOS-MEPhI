
all:
	echo "Choice the option"

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
	./7 6_sparsed_file.c

clean:
	rm -r 1 2_3 4_5 6 7 tmp