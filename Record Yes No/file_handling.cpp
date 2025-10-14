 

#include "stdafx.h"
#include "stdio.h"
#include "process.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int x,i = 0;
	int arr[20000] = {0};
	int size;
	int err;
	
	FILE *f1 = NULL;
	err = fopen_s(&f1 , "sample_input.txt" , "r");
	if (err !=NULL){
		printf( "\n Cannot Open");
		exit(1);
	}
	while(!feof(f1)){
		if (fscanf_s(f1 , "%d" , &x) == 1){
			printf("%d\n" , x);
			arr[i] = x;
			i++;
		}
	}
	fclose(f1);
	size = i;

	FILE *f2 = NULL;
	err = fopen_s(&f2 , "sample_output.txt" , "w");
	if (err !=NULL){
		printf( "\n Cannot Open");
		exit(1);
	}
	for(i = 0;i < size;i++) {
		fprintf(f2,"%d \n",arr[i]);
	}
	fclose(f2);

}

