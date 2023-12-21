ccpp build -r
mproc ./bin/release/brainfuck ../speed-test.b < ../input > res
diff ../expected res
./bin/release/brainfuck -T ../speed-test.b -o test.c
cc test.c -O3 -o test.out
mproc ./test.out < ../input > res
diff ../expected res
