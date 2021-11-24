/****************************************************************************
 A local department store chain wants to set up an online credit system. 
 When a person wishes to purchase a product, the user must give their 
 account number and the purchase amount to the program. The program will
 take this data and locate that customer in the file. If it does not find 
 this number in the file, the program must create a new record. The program
 must print the original file and the post-transaction file updated 
 ****************************************************************************/
 
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<math.h>
/*Functions*/
void binary(FILE *);
void read(FILE *);
void print(FILE *);
void deletee(FILE *);
/*Struct*/
struct clientData {
	char customerName[20];
	int accountNumber;
	float currentBalance;
	float creditLimit;
};
/*Main*/
int main (){
	FILE *ptr;   
	int choice;
	do{
		printf("Enter:\n\t1.Create the binary file\n\t2.On-line Credit System\n\t3.Delete Record\n\t4.Exit\n");
		printf("\nEnter (1 or 2 or 3 or 4): ");
		scanf("%d",&choice);
		printf("\n");
			switch(choice){
				
				case 1:
					binary(ptr);
					system("pause");
					break;
					
				case 2:
					if((ptr=fopen("ACCT.dat","r+"))==NULL){
						printf("File could not be opened.\n");
					}
					else{
						read(ptr);
						fclose(ptr);	
					}
					system("pause");
					break;
					
				case 3:
					deletee(ptr);
					system("pause");
					break;
					
				case 4 :
					printf("THANK YOU!!\n");
					system("pause");
					break;
				
				default :
					printf("WRONG INPUT\n");
					system("pause");
					break;
			}
			system("CLS");
	}while (choice!=4);
	print(ptr);		
	return 0;	
}
/*This function checks if the record exists. If it does, it shows the right 
 message to the screen and updates the binary file accordingly to the data.
 If the record does not exist, the algorithm creates a new record by asking
 the user to enter the data.*/
void read(FILE *ptr){
	char enter1;
	int account_number=0;
	static float amount;
	SetConsoleOutputCP(1253);
	char customer_name[20]="" ,c,enter;
	struct clientData client={"",0,0.0,0.0};
	do{
		printf("ENTER ACCOUNT NUMBER: ");
		scanf("%d",&account_number);		
		while(account_number%50!=0 || account_number>20000){
			if(account_number<0){
				return 0;
			}
			else{
				printf("WRONG INPUT, RE-ENTER ACCOUNT NUMBER: ");     
				scanf("%d",&account_number);
				if(account_number<0){
					return 0;
				}
			}
		}
		fseek(ptr,(account_number/50-1)*sizeof(struct clientData),SEEK_SET);
		fread(&client,sizeof(struct clientData),1,ptr); 			
		if(client.accountNumber==account_number){
			scanf("%c",&enter1);
			printf("ENTER CUSTOMER NAME: ");
			gets(customer_name);
			while(strcmp(client.customerName,customer_name)){
				printf("ACCOUNT NUMBER AND CUSTOMER NAME DO NOT MATCH - RE-ENTER DATA\n");
				read(ptr);
				break;
			}
			printf("ENTER AMOUNT OF PURCHASE: ");
			scanf("%f",&amount);			
			while(amount<0){
				printf("WRONG INPUT, RE-ENTER AMOUNT OF PURCHASE: ");
				scanf("%f",&amount);
			}			
			if(fabs(client.creditLimit - client.currentBalance) >=amount){  
				printf("DEAR %s\n",client.customerName);
				printf("      \tTHANK YOU FOR THE BUSINESS\n");
				client.currentBalance=client.currentBalance+amount;
				fseek(ptr,(account_number/50-1)*sizeof(struct clientData),SEEK_SET);
				fwrite(&client,sizeof(struct clientData),1,ptr); 
			}
			else{
				printf("DEAR %s\n",client.customerName);
				printf("      \tWE CANNOT AUTHORIZE THIS PURCHASE AT THIS TIME AS IT\n");
				printf("      \tWOULD PUT YOU € %.2f OVER YOUR CREDIT LIMIT\n",fabs(client.creditLimit - client.currentBalance-amount));
			}
		}	
		else{
			printf("ENTER AMOUNT OF PURCHASE: ");
			scanf("%f",&amount);
			while(amount<0){
				printf("WRONG INPUT, RE-ENTER AMOUNT OF PURCHASE: ");
				scanf("%f",&amount);
			}
			printf("IS THIS A NEW ACCOUNT NUMBER? (Y OR N): ");
			scanf("%s",&c);
			if(c=='N'){
				printf("INVALID ACCOUNT NUMBER - RE-ENTER DATA\n");
				read(ptr);
			}
			else{
				scanf("%c",&enter);
				printf("ENTER CUSTOMER NAME: ");
				gets(client.customerName);
				printf("CUSTOMER CREDIT LIMIT: ");
				scanf("%f",&client.creditLimit);
				client.accountNumber=account_number;
				fseek(ptr,(account_number/50-1)*sizeof(struct clientData),SEEK_SET);
				fwrite(&client,sizeof(struct clientData),1,ptr);
				if(client.creditLimit - client.currentBalance >=amount){
					printf("DEAR %s\n",client.customerName);
					printf("      \tTHANK YOU FOR THE BUSINESS\n");
					client.currentBalance=client.currentBalance+amount;
					fseek(ptr,(account_number/50-1)*sizeof(struct clientData),SEEK_SET);
					fwrite(&client,sizeof(struct clientData),1,ptr); 
				}
				else{
					printf("DEAR %s\n",client.customerName);
					printf("      \tWE CANNOT AUTHORIZE THIS PURCHASE AT THIS TIME AS IT\n");
					printf("      \tWOULD PUT YOU € %.2f OVER YOUR CREDIT LIMIT\n",fabs(client.creditLimit - client.currentBalance-amount));
				}	
			} 
		}
	}while(1);
}
/*This function deletes a record. The user enters a specific account number, 
 finds it and delete it from the binary file.*/
void deletee(FILE *ptr){
	struct clientData client,
 	blank={"",0,0.0,0.0};
 	int num;
  	if((ptr=fopen("ACCT.dat","r+"))==NULL){
		printf("FILE COULD NOT BE OPENED\n");   
	}
	else{
		printf("ENTER ACCOUNT NUMBER TO DELETE: ");
		scanf("%d",&num);
		fseek(ptr,(num/50-1)*sizeof(struct clientData),SEEK_SET);
		fread(&client,sizeof(struct clientData),1,ptr); 
		if(client.accountNumber==0){
			printf("THE ACCOUNT NUMBER %d DOES NOT EXIST\n",num);
		}
		else{
			fseek(ptr,(num/50-1)*sizeof(struct clientData),SEEK_SET);
			fwrite(&blank,sizeof(struct clientData),1,ptr);
		}	
	}
	fclose(ptr);	
}
/*This function creates a new text file and prints the data from the binary
 file into the text file.*/
void print(FILE *ptr){
    FILE *ptr2;
    struct clientData client={"",0,0.0,0.0};
    if((ptr=fopen("ACCT.dat","r"))==NULL){
		printf("FILE COULD NOT BE OPENED\n");   
	}
	else{
		if((ptr2=fopen("client.txt","w"))==NULL){
			printf("FILE COULD NOT BE OPENED\n");   
		}
    	else{	
    		fread(&client,sizeof(struct clientData),1,ptr); 
      		fprintf(ptr2,"%-25s%-15s%-14s%12s\n","NAME","NUMBER","BALANCE","LIMIT");
      	 	while(!feof(ptr)){
      	   		if(client.accountNumber!=0){
      	    		fprintf(ptr2,"%-25s%-15d%-14.2f%12.2f\n",client.customerName,client.accountNumber,client.currentBalance,client.creditLimit);
			}
			fread(&client,sizeof(struct clientData),1,ptr);
  	     	}
    	    fclose(ptr2);
   		}
   		fclose(ptr);
	}
} 
/*This function creates the binary file and puts the data into the file.*/
void binary(FILE *ptr){
	int i=0;
	char enter,exit;  
    struct clientData client={"",0,0.0,0.0};

	if((ptr=fopen("ACCT.dat","w"))==NULL){  
		printf("FILE COULD NOT BE OPENED\n");   
	}
	else{
		for(i=0;i<200;i++){
			fwrite(&client,sizeof(struct clientData),1,ptr);
		}
	do{
		scanf("%c",&enter);
		printf("NAME: ");
		gets(client.customerName);
		do{
			printf("ACCOUNT NUMBER: ");
			scanf("%d",&client.accountNumber);
			if(client.accountNumber%50!=0 ||client.accountNumber<50 || client.accountNumber>20000 ){
				printf("WRONG INPUT, RE-ENTER ");
			}
		}while(client.accountNumber%50!=0 ||client.accountNumber<50 || client.accountNumber>20000);
		printf("BALANCE: ");
		scanf("%f",&client.currentBalance);
		printf("CREDIT LIMIT: ");
		scanf("%f",&client.creditLimit);
		fseek(ptr,(client.accountNumber/50-1)*sizeof(struct clientData),SEEK_SET);
		fwrite(&client,sizeof(struct clientData),1,ptr);
		printf("DO YOU WANT TO CONTINUE? (Y OR N ): "); 
		scanf("%s",&exit);
		
	}while(exit!='N');
	fclose(ptr);
	}
}
