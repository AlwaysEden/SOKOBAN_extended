
F='pushpush'

gcc ${F}.c -o ${F} `pkg-config --libs --cflags gtk+-3.0`

