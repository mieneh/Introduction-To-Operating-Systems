#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>


#define UPPER_LIM 1000 //so ngau nhien lon nhat
int isPrime(int n); //ham kiem tra so nguyen to
int isSquare(int n); // ham kiem tra so chinh phuong

// khai bao cau truc de luu tru du lieu
struct data{
	long mesg_type;
	int n; // so phan tu cua mang
	int a[UPPER_LIM]; // cac phan tu  cua mang
}array;
//thread de kiem tra phan tu nao la nguyen to
void* prime(void* array){
	struct data *ap = (struct data*) array;
	int i ;
	printf("Prime of n is\n");
	for(i = 0; i < ap -> n; i++){
		if(isPrime(ap -> a[i]) == 1){
			printf("%d ", ap->a[i]);
		}
	}
}
//thread de kiem tra phan tu nao so chinh phuong
void* chan(void* array){
	struct data *ap = (struct data*) array;
	int i ;
	printf("\r\nChan of n is");
	for(i = 0; i < ap -> n; i++){
		if(isChan(ap -> a[i]) == 1){
			printf("%d ", ap->a[i]);
		}
	}
}

void Save_file(struct data array)
{
	FILE *f;
	f = fopen("ketqua", "w");
	int i;
	fputs("ket qua lam bai la!", f);
	for(i  = 0; i < array.n; i++){
		printf("%d ", array.a[i]);
		fprintf(f,"%d ", array.a[i]);	
	}
	fprintf(f,"\r\n");
	printf("\n");
	//f.close();
}

int main()
{
	// Hay viet message queue de nhan gia tri array tu main gui qua
	key_t key;
	int msgid;
	pthread_t tid[2];
	key = ftok("msg.txt", 1);
	msgid = msgget(key, 0666 | IPC_CREAT);
	msgrcv(msgid, &array, sizeof(array), 1, 0);
	printf("%d\n", array.a[0]); 
	int i =0;	
	for(i =0; i < array.n; i++){
		printf("%d ", array.a[i]);
	}
	Save_file(array);
	// Thuc hien goi 2 thread thuc thi nhu da khai bao
	pthread_create(&tid[0], NULL , prime, (void*) &array);
	pthread_create(&tid[1], NULL , chan, (void*) &array);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	return 0;
}
// Ham kiem tra so nguyen to
int isPrime(int n)
{
	if (n<2)
		return 0;
	int square=(int)sqrt(n);
	int i;
	for (i=2;i<=square;i++)
		if (n%i==0)
			return 0;
	return 1;
}
// Ham kiem tra so chinh phuong
int isSquare(int n)
{
	int sqr=sqrt(n);
	if (sqr*sqr==n)
		return 1;
	else
		return 0;
}

int isChan(int n){
	if (n<2)
		return 0;
	//int square=(int)sqrt(n);
	int i;
	if(n % 2 != 0) return 0; 
	return 1;
}
