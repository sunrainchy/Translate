all: chy
chy: Gtrans.c
	gcc -o $@ $< -lcurl
clean:
	rm chy
