#include <iostream>
using namespace std;

// 实现strcpy,将src字符串复制到dest,包括尾部的\0
char *my_strcpy(char *dest, const char *src) {
    if(src==NULL || dest==NULL) return NULL;
    char *strDest = dest;
    char *strSrc = (char*)src;
    while(*strSrc!='\0') {
        *strDest++ = *strSrc++;
    }
    *strDest = '\0';
    return dest;
}
// 实现memcpy内存复制函数，对复制的内容没有限制，可能存在内存重叠引发问题
void *my_memcpy(void *memTo, const void *memFrom, size_t size) {
    if(memTo==NULL || memFrom==NULL) return NULL;
    char *tempFrom = (char*)memFrom;
    char *tempTo = (char*)memTo;
    while(size--) {
        *tempTo++ = *tempFrom++;
    }
    return memTo;
}
// memmove里面则判断了内存重叠的情况，当内存重叠时从后往前复制，以确保复制正常处理。
void *my_memmove(void *memTo, const void *memFrom, size_t size) {
    if(memTo==NULL || memFrom==NULL) return NULL;
    char *tempFrom = (char*)memFrom;
    char *tempTo = (char*)memTo;
    if(tempTo<=tempFrom || tempTo>=tempFrom+size) { //从前往后复制，则不会出现覆盖memFrom中没有复制的内容
        while(size--) {
            *tempTo++ = *tempFrom++;
        }
    } else { //从后往前复制，则不会出现覆盖src中没有复制的内容
        tempFrom = tempFrom+size-1;
        tempTo = tempTo+size-1;
        while(size--) {
            *tempTo-- = *tempFrom--;
        }
    }
    return memTo;
}

int main() {
    char a[] = "123";
    char *b = (char*)malloc(sizeof(10));
//    my_strcpy(b,a);
//    my_memcpy(b,a,2);
    my_memmove(b,a,2);
    printf("%s\n", b);
    free(b);
}
