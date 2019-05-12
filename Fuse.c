#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

static const char *dirpath = "/home/zidan";

struct node {
    char *name, *path;
    struct node *next;
};

struct queue {
    struct node *head, *tail;
    int size;
};

void push(struct queue *q, char *name, char *path) {
    struct node* box = (struct node*) malloc(sizeof(struct node));
    box->next = NULL;
    box->name = (char *) malloc(strlen(name) + 10);
    box->path = (char *) malloc(strlen(path) + 10);
    strcpy(box->name, name);
    strcpy(box->path, path);

    if(q->head == NULL) {
        q->head = box;
        q->tail = box;
    }
    else {
        q->tail->next = box;
        q->tail = box;
    }
    q->size++;
    return;
}

char* pop(struct queue *q) {
    if(q->head == NULL) return NULL;
    struct node* temp = q->head;
    char *res = (char *) malloc(strlen(temp->path) + 10);
    strcpy(res, temp->path);
    q->head = temp->next;
    q->size--;
    free(temp);
    return res;
}

struct queue *q, *q2;

void bfs() {
    push(q, (char*)"", (char *)dirpath);
    while(q->size > 0) {
        char *currentPath = (char *) malloc(strlen(q->head->path) + 10);
        sprintf(currentPath, "%s", pop(q));
        DIR *dp;
        struct dirent *de;
        dp = opendir(currentPath);
        if (dp == NULL)
            return;

        while ((de = readdir(dp)) != NULL) {
            if(de->d_type == DT_DIR) {
                // Harap merubah strcmpt(de->d_name, "test") sesuai dengan nama folder mount nya, terima kasih
                if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0 && strcmp(de->d_name, "mountable") != 0 && de->d_name[0] != '.') {
                    char *newPath = (char *) malloc(strlen(currentPath) + strlen(de->d_name) + 10);
                    sprintf(newPath, "%s/%s", currentPath, de->d_name);
                    push(q, "", newPath);
                }
            }
            else {
                int panjang = strlen(de->d_name);
                if(strlen(de->d_name) <= 4 || de->d_name[0] == '.' || !(de->d_name[panjang - 4] == '.'  && de->d_name[panjang - 3] == 'm' && de->d_name[panjang - 2] == 'p' && de->d_name[panjang - 1] == '3')) continue;
                char *fullPath = (char *) malloc(strlen(currentPath) + strlen(de->d_name) + 10);
                sprintf(fullPath, "%s/%s", currentPath, de->d_name);
                char *newName = (char *) malloc(strlen(de->d_name) + 10);
                sprintf(newName, "/%s", de->d_name);
                push(q2, newName, fullPath);
            }
        }
        closedir(dp);
    }
    return;
}

char* getFileName(char *fullPath) {
    int panjang = strlen(fullPath), i, j = 0;
    char *res = (char *) malloc(strlen(fullPath) + 10);
    for(i = panjang - 1; i >= 0; i--) {
        if(fullPath[i] == '/') break;
    }
    for(i = i + 1; i < panjang; i++) {
        res[j] = fullPath[i];
        j++;
    }
    res[j] = '\0';
    return res;
}

static void *xmp_init(struct fuse_conn_info *conn)
{
    bfs();
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
    char fpath[1000];
    struct node* temp = q2->head;
    int flag = 0;
    while(temp != NULL) {
        if(strcmp(temp->name, path) == 0) {
            flag = 1;
            break;
        }
        temp = temp->next;
    }
    if(!flag) sprintf(fpath,"%s%s",dirpath,path);
    else sprintf(fpath, "%s", temp->path);
    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    (void) offset;
    (void) fi;
    struct node* temp2 = q2->head;
    while(temp2 != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        char *temp = temp2->path;
        int panjang = strlen(temp);
        if(panjang > 20 && temp[panjang - 4] == '.' && temp[panjang - 3] == 'm' && temp[panjang - 2] == 'p' && temp[panjang - 1] == '3') {
            char *fileName = getFileName(temp);
            if(filler(buf, fileName, &st, 0) != 0) break;
        }
        temp2 = temp2->next;
    }

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi)
{
    char fpath[1000];
    // if(strcmp(path,"/") == 0)
    // {
    //  path=dirpath;
    //  sprintf(fpath,"%s",path);
    // }
    // else sprintf(fpath, "%s%s",dirpath,path);
    int flag = 0;
    struct node* temp = q2->head;
    while(q2 != NULL) {
        if(strcmp(temp->name, path) == 0) {
            flag = 1;
            break;
        }
        temp = temp->next;
    }
    if(!flag) sprintf(fpath, "%s%s", dirpath, path);
    else sprintf(fpath, "%s", temp->path);
    int res = 0;
  int fd = 0 ;

    (void) fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static struct fuse_operations xmp_oper = {
    .init       = xmp_init,
    .getattr    = xmp_getattr,
    .readdir    = xmp_readdir,
    .read       = xmp_read,
};

int main(int argc, char *argv[])
{
    q = (struct queue*) malloc(sizeof(struct queue));
    q->head = NULL; q->tail = NULL;
    q->size = 0;
    q2 = (struct queue*) malloc(sizeof(struct queue));
    q2->head = NULL; q2->tail = NULL;
    q2->size = 0;
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}