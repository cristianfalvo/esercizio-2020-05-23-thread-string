/*
 * esercizio-2020-05-23-thread-string.c
 *
 *  Created on: May 21, 2020
 *      Author: marco
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>
#include <pthread.h>

void * thread_vowel(void * arg);
void * thread_cons(void * arg);
void * thread_space(void * arg);
int equals_any(char i, char * charset);
int equals_none(char i, char * charset);

/*
 * siccome passo la stringa come parametro, non c'è problema di sincronizzazione (?)
 *
 * */

//int vowels;
//int cons;
//int spaces;

int main() {

	pthread_t threads[3];

	char * content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Mattis rhoncus urna neque viverra justo nec ultrices. Pretium quam vulputate dignissim suspendisse in est ante. Vitae congue mauris rhoncus aenean. Blandit cursus risus at ultrices mi. Ut lectus arcu bibendum at varius vel pharetra vel. Etiam non quam lacus suspendisse faucibus interdum posuere. Eget sit amet tellus cras adipiscing enim eu turpis egestas. Lectus magna fringilla urna porttitor rhoncus dolor purus non. Sit amet consectetur adipiscing elit duis tristique sollicitudin nibh. Nec tincidunt praesent semper feugiat nibh. Sapien pellentesque habitant morbi tristique senectus et netus et malesuada.";

	int s;

	s = pthread_create(&threads[0], NULL, thread_vowel, content);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&threads[1], NULL, thread_cons, content);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&threads[2], NULL, thread_space, content);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	void * a = malloc(sizeof(int));
	void * b = malloc(sizeof(int));
	void * c = malloc(sizeof(int));

	s = pthread_join(threads[0], a);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(threads[1], b);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	s = pthread_join(threads[2], c);

	if (s != 0) {
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}

	printf("a:%p\n", a);

	int * vowels = (int *) a;
	int * cons = (int *) b;
	int * spaces = (int *) c;


	printf("Vocali trovate:%d\nConsonanti trovate:%d\nSpazi e newline trovati:%d\nBye\n", *vowels, *cons, *spaces);

	return 0;
}

void * thread_vowel(void * arg){
	char * content = (char *) arg;
	int * output = malloc(sizeof(int));
	if (output == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	int counter = 0;
	char * chars = {"aeiouAEIOU"};

	for(int i = 0; i < strlen(content); i++){

		if(equals_any(content[i], chars)){
			counter++;
			//printf("Vowel:%d\n", *counter);
		}
	}

	*output = counter;

	printf("finito spaces; valore finale:%d, %d\n", counter, *output);
	return (void *)output;
}

void * thread_cons(void * arg){
	char * content = (char *) arg;
	int * output = malloc(sizeof(int));
	if (output == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	int counter = 0;
	char * chars_bad = {"aeiouAEIOU \n,."};

	for(int i = 0; i < strlen(content); i++){

			if( equals_none(content[i], chars_bad)){
				counter++;
				//printf("Cons:%d\n", *counter);
			}
		}
	*output = counter;

	printf("finito spaces; valore finale:%d, %d\n", counter, *output);
	return (void *)output;
}

void * thread_space(void * arg){
	char * content = (char *) arg;
	int * output = malloc(sizeof(int));
	if (output == NULL){
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	int counter = 0;

	char * chars = {" \n"};

	for(int i = 0; i < strlen(content); i++){

			if( equals_any(content[i], chars)){
				counter++;
				//printf("Spaces:%d\n", *counter);
			}
		}
	*output = counter;

	printf("finito spaces; valore finale:%d, %d\n", counter, *output);
	return (void *)output;
}

int equals_any(char c, char * charset){

	for(int i = 0; i < strlen(charset); i++){
		if (c == charset[i]) return 1;
	}
	return 0;
}

int equals_none(char c, char * charset){
	for(int i = 0; i < strlen(charset); i++){
		if (c == charset[i]) return 0;
	}
	return 1;
}
