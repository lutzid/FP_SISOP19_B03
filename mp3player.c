#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ao/ao.h>
#include <mpg123.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

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
char songList[1000][100] = {};
int counter = 0;
int stop = 0;

void* play(void *arg){

	paused = 0;
	char *name = ((char*)arg);
	sprintf(fpath, "%s%s", dirpath, name);

	ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new("\0", &err);
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
			else if(stop == 1) break;
			else break;
		}
		sleep(0.01);
	}


    /* clean up */
    // free(buffer);
    // ao_close(dev);
    // mpg123_close(mh);
    // mpg123_delete(mh);
    // mpg123_exit();
    // ao_shutdown();
}

void cleanup(){
	free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}

void pause_resume(){
	if(paused == 0)
		paused = 1;
	else
		paused = 0;
	// printf("%d", paused);
	// return paused;
}

void* next(){

}

void* prev(){

}

void list(){
	// DIR *dir;
	// struct dirent *ent;

	// if ((dir = opendir (dirpath)) != NULL) {
	//   /* print all the files and directories within directory */
	//   while ((ent = readdir (dir)) != NULL) {
	//   	if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
	//   		continue;
	//     else 
	//     	printf ("%s\n", ent->d_name);
	//   }
	//   closedir (dir);
	// } 
	// else {
	//   /* could not open directory */
	//   perror ("");
	// }
	int i;
	for(i = 1; i <= counter; i++)
		printf("%d. %s\n", i, songList[i]);
}



void prelist(){
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (dirpath)) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
	  	if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
	  		continue;
	    else {
	    	counter++;
	    	sprintf(songList[counter], "%s", ent->d_name);
	    }
	  }
	  closedir (dir);
	} 
	else {
	  /* could not open directory */
	  perror ("");
	}
}

int main(){
	int choose;
	int id;
	char lagu[100];
	char nama[100];
	prelist();
	// printf("%d", counter);
	// system("clear");
	while(1){
		printf("================================mp3Player by B03================================\n");
		printf("Lagu yang sedang diputar: %s\n", lagu);
		printf("Menu:\n");
		printf("1. Play Lagu\n");
		printf("2. Pause & Resume\n");
		printf("3. Next\n");
		printf("4. Prev\n");
		printf("5. List Lagu\n");
		printf("6. Exit\n");
		printf("\n");
		scanf("%d", &choose);
		if(choose == 1){
			// system("clear");
			// strcpy(lagu, "");
			// sprintf(nama, " ");
			// printf("nama =%saaa\n", nama);
			// memset(lagu, '\0', sizeof(lagu));
			printf("Masukkan id lagu\n");
			scanf("%d", &id);
			// strncpy(nama, songList[id]+3, strlen(songList[id]) - 3);
			sprintf(nama, "%s", songList[id]);
			// printf("nama strn = %s\n", nama);

			sprintf(lagu, "%s", nama);
			// printf("%s", lagu);
			pthread_create(&(tid[1]), NULL, &play, lagu);
			system("clear");
		}
		if(choose == 2){
			pause_resume();
			system("clear");
		}
		if(choose == 3){
			system("clear");
			if(id == counter)
				id = 0;
			// stop = 1;
			// strcpy(lagu, "");
			// sprintf(nama, " ");
			// printf("nama =%saaa\n", nama);

			// memset(lagu, '\0', sizeof(lagu));
			id += 1;
			// printf("id = %d", id);
			// cleanup();
			pthread_cancel(tid[1]);
			// strncpy(nama, songList[id]+3, strlen(songList[id]) - 3);
			sprintf(nama, "%s", songList[id]);
			// printf("song list = %s\n", songList[id]);
			// printf("nama strn = %s\n", nama);

			sprintf(lagu, "%s", nama);			
			// printf("%s\n", lagu);
			pthread_create(&(tid[1]), NULL, &play, lagu);
		}
		if(choose == 4){
			system("clear");
			if(id == 1)
				id = counter + 1;
			// stop = 1;
			// strcpy(lagu, "");
			sprintf(nama, " ");
			// printf("nama =%saaa\n", nama);

			// memset(lagu, '\0', sizeof(lagu));
			id -= 1;
			// printf("id = %d", id);
			// cleanup();
			pthread_cancel(tid[1]);
			// strncpy(nama, songList[id]+3, strlen(songList[id]) - 3);
			sprintf(nama, "%s", songList[id]);
			// printf("nama strn = %s\n", nama);
			sprintf(lagu, "%s", nama);
			// printf("%s\n", lagu);
			pthread_create(&(tid[1]), NULL, &play, lagu);
		}
		if(choose == 5){
			system("clear");
			printf("List Lagu: \n");
			list();
		}
		if(choose == 6){
			cleanup();
			break;
		}
		
	}
}