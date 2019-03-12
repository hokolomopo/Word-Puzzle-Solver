#include<stdio.h>
#include <stdlib.h>

#define internal(A) ((A->d) != NULLdigit)
#define NULLitem NULL
#define NULLdigit '\0'
#define key(A) (A)

typedef struct STnode* link;

struct STnode {
     char* item;
     int d;
     link l, m, r;
};


static link* heads;

void STinit() {
     heads = malloc(26 * sizeof(link));
}

int digit(char* v, int w){
    return v[w];
}

int eq(char*c, char* v){
    int i = 0;

    if(c == NULL && v == NULL)
        return 1;
    if((c != NULL && v == NULL) || (c == NULL && v != NULL))
        return 0;

    while(c[i] != '\0'){
        if(c[i] != v[i])
            return 0;
        if(v[i] == '\0')
            return 0;
    }

    return 1;
}

link NEW(int d){
    link x = malloc(sizeof *x);
    x->d = d;
    x->l = NULL;
    x->m = NULL;
    x->r = NULL;
    return x;
 }

 link NEWx(link h, int d){
    link x = malloc(sizeof *x);
    x->item = NULLitem;
    x->d = d;
    x->l = NULL;
    x->r = NULL;
    x->m = h;
    return x;
 }

 char* searchR(link h, char* v, int w) {
    int i = digit(v, w);
    if (h == NULL)
        return NULLitem;
    if (internal(h)){
        if (i < h->d)
            return searchR(h->l, v, w);
        if (i == h->d)
            return searchR(h->m, v, w+1);
        if (i > h->d)
            return searchR(h->r, v, w);
    }
    if(eq(v, key(h->item)))
        return h->item;
    return NULLitem;
 }

char* STsearch(char* v){
    return searchR(heads[digit(v, 0)], v, 1);
}

link split(link p, link q, int w){
    int pd = digit(p->item, w),
    qd = digit(q->item, w);
    link t = NEW(qd);
    if (pd < qd) {
        t->m = q;
        t->l = NEWx(p, pd);
    }
    if (pd == qd) {
        t->m = split(p, q, w+1);
    }
    if (pd > qd) {
        t->m = q;
        t->r = NEWx(p, pd);
    }
    return t;
}

link insertR(link h, char* item, int w){
    char* v = key(item);
    int i = digit(v, w);
    if (h == NULL)
        return NEWx(NEW(NULLdigit), i);
    if (!internal(h))
        return split(NEW(NULLdigit), h, w);
    if (i < h->d)
        h->l = insertR(h->l, v, w);
    if (i == h->d)
        h->m = insertR(h->m, v, w+1);
    if (i > h->d)
        h->r = insertR(h->r, v, w);
    return h;
 }

void STinsert(char* key){
    int i = digit(key, 0);
    heads[i] = insertR(heads[i], key, 1);
}
