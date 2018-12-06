//
//  main.c
//  matrixMultiThreaded
//
//  Created by Khaled Mohab on 11/16/18.
//  Copyright © 2018 Khaled Mohab. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
struct size
{
    int row,ind,column;
};
int **matrix1;
int **matrix2;
int **resultant;

void* multiply(void *arg)
{
    struct size* matrix=(struct size*)arg;
    int sum=0;
    for(int i=0;i<matrix->ind;i++)
        sum+= matrix1[matrix->row][i] * matrix2[i][matrix->column];
    resultant[matrix->row][matrix->column]=sum;
    pthread_exit(0);
}
void* multiplyrow(void *arg)
{
    struct size* matrix=(struct size*)arg;
	for(int j=0;j<matrix->column;j++){
    		int sum=0;
    		for(int i=0;i<matrix->ind;i++)
		{
       	 		sum+= matrix1[matrix->row][i] * matrix2[i][j];
		}
		resultant[matrix->row][j]=sum;
	}
    
    pthread_exit(0);
}
void printMatrix(int row,int col, int b)
{
    printf("resultant matrix:\n ");
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++)
            printf("%d ",resultant[i][j]);
        printf("\n ");
    }
}
int main(int argc, const char * argv[]) {
    int a,b,c;
    int *ptr1,*ptr2,*ptr3;
    clock_t start, end;
    double cpu_time_used;
    do
    {
        a=rand()%10;
        b=rand()%10;
        c=rand()%10;
    }while(a==0||b==0||c==0);
	a=500;
	b=500;
	c=500;
    matrix1=(int**) malloc(sizeof(int*)*a + sizeof(int)*(a*b));
    matrix2=(int**) malloc(sizeof(int*)*b + sizeof(int)*(b*c));
    resultant=(int**) malloc(sizeof(int*)*a + sizeof(int)*(a*c));
    ptr1 = ((matrix1 + a));
    ptr2 = (matrix2 + b);
    ptr3 = (resultant + a);
    for(int i = 0; i < a; i++)
        matrix1[i] = (ptr1 + b * i);
    for(int i = 0; i < b; i++)
        matrix2[i] = (ptr2 + c * i);
    for(int i = 0; i < a; i++)
        resultant[i] = (ptr3 + c * i);
    struct size index[a][c];
    struct size indexrow[a];
    for (int i=0; i<a; i++)
        for (int j=0; j<c; j++){
            index[i][j].ind=b;
            index[i][j].row=i;
            index[i][j].column=j;
        }
    for (int i=0; i<a; i++)
	{
            indexrow[i].ind=b;
            indexrow[i].row=i;
            indexrow[i].column=c;
        }

    pthread_t tids[a][c];
    pthread_t tidsrow[a];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (int i=0; i<a; i++)
        for (int j=0; j<b; j++)
            matrix1[i][j]=rand()%1000;
    for (int i=0; i<b; i++)
        for (int j=0; j<c; j++)
            matrix2[i][j]=rand()%1000;
    for (int i=0; i<a; i++)
        for (int j=0; j<c; j++)
            resultant[i][j]=1;
    start=clock();
    for (int i=0; i<a; i++)
        for (int j=0; j<c; j++)
            pthread_create(&tids[i][j], &attr, multiply, (void*)&index[i][j]);
    for (int i=0; i<a; i++)
        for (int j=0; j<c; j++)
        pthread_join(tids[i][j], NULL);
    end=clock();
    cpu_time_used=((double)(end-start))/ CLOCKS_PER_SEC;
    printf("cell by cell took time : %f \n ",cpu_time_used);
    printMatrix(a, c, b);
    start=clock();
    for (int i=0; i<a; i++)
        for (int j=0; j<c; j++)
            pthread_create(&tidsrow[i], &attr, multiplyrow, (void*)&indexrow[i]);
    for (int i=0; i<a; i++)
        for (int j=0; j<c; j++)
        pthread_join(tidsrow[i], NULL);
    end=clock();
    cpu_time_used=((double)(end-start))/ CLOCKS_PER_SEC;
    printf("row by row took time : %f \n ",cpu_time_used );
    printMatrix(a, c, b);
    return 0;
}
