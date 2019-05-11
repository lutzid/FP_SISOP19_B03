#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ao/ao.h>
#include <mpg123.h>

#define BITS 8

mpg123_handle *mh;
unsigned char *buffer;
size_t buffer_size;
size_t done;
int err;

int driver;
ao_device *dev;

ao_sample_format format;
int channels, encoding;
long rate;

pthread_t tid[6];
char dirpath[1000] = "/home/zidan/mount/";
char fpath[1000];
int paused;
int playing;

void* play(void *arg){
	playing = 1;
	paused = 0;
	char *name = ((char*)arg);
	sprintf(fpath, "%s%s", dirpath, name);

	ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, fpath);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    
	while (1){
		if(paused == 0){
			if(mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
				ao_play(dev, (char*)buffer, done);
			else break;
		}
		sleep(0.01);
	}


    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}

void pause_song(){
	paused = 1;
	// printf("%d", paused);
	// return paused;
}

void resume_song(){
	paused = 0;
	// printf("%d", paused);
	// return paused;
}

void* next(){

}

void* prev(){

}

void* list(){

}

int main(){
	int choose;
	char lagu[100];
	while(1){
		printf("===============mp3Player by B03===============\n");
		printf("Menu:\n");
		printf("1. Play Lagu\n");
		printf("2. Pause\n");
		printf("3. Resume\n");
		printf("4. Next\n");
		printf("5. Prev\n");
		printf("6. List Lagu\n");
		printf("7. Exit\n");
		scanf("%d", &choose);
		if(choose == 1){
			printf("Masukkan nama lagu\n");
			scanf("%s", lagu);
			pthread_create(&(tid[1]), NULL, &play, lagu);
		}
		if(choose == 2){
			// pthread_create(&(tid[2]), NULL, &pause_song, NULL);
			pause_song();
		}
		if(choose == 3){
			// pthread_create(&(tid[3]), NULL, &resume_song, NULL);
			resume_song();
		}
		if(choose == 4){

		}
		if(choose == 5){

		}
		if(choose == 6){
			
		}
		if(choose == 7){
			break;
		}
	}

}