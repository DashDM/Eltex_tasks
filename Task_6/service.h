#ifndef __SERVICE_H__
#define __SERVICE_H__
#define field_size 10

struct abonent {
    char name[field_size];
    char second_name[field_size];
    char tel[field_size];
};

void add(struct abonent*, int);
void del(struct abonent*, int*, int);
void find(struct abonent*, int, char*);
void out(struct abonent*, int);
#endif
