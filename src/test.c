#include<stdio.h>
int main(){
    int a[] ={1,2,3,4,5,6};
    int *b =&a[1];
    int i=0;
    for(i=0;i<5;i++){
        printf("%d\n",b[i]);
    }
}
