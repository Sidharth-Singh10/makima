rm ./main
cc -std=c99 -Wall main.c mpc.c eval.c errors.c -ledit -lm -o main
chmod +x main
./main