#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct input
{
    char label[100];
    char opcode[100];
    char operand[100];
};
struct str
{
    char *row;
};
struct check
{
    int addr;
    char symbol[100];
};
struct opdata
{
    char opc[100];
    char hex[10];
};

void readinput(struct input list[100],struct str  sav[100],int c1,int *val)
{
    FILE *fp1;
    char word[100];
    fp1=fopen("SIC_inputs.txt","r");
    if(fp1==NULL)
    {
        printf("Unable to open the file!");
    }
     while(fgets(word,sizeof(word),fp1))
    {
       char * token = strtok(word," \t\n");
        int count1=0;
        while(token!=NULL)
        {

        sav[count1].row=token;
            count1++;
            token = strtok(NULL," \t\n");

        }
        if(count1==3)
        {
            strcpy(list[c1].label,sav[0].row);
            strcpy(list[c1].opcode,sav[1].row);
              strcpy(list[c1].operand,sav[2].row);
        }
         if(count1==2)
        {

            strcpy(list[c1].opcode,sav[0].row);
            strcpy(list[c1].operand,sav[1].row);
        }
        if(count1==1)
        {
             strcpy(list[c1].opcode,sav[0].row);
        }

        c1++;


    }
    *val=c1;
    fclose(fp1);



}
void readoptab(struct opdata op[100],struct str  sav[100],int c2,int *val)
{
    FILE *fp2;
    char word[100];
        fp2=fopen("optab.txt","r");
    if(fp2==NULL)
    {
        printf("Unable to open the file!");
    }
    while(fgets(word,sizeof(word),fp2))
    {
       char * token = strtok(word," \t\n");
        int count1=0;
        while(token!=NULL)
        {
        sav[count1].row=token;
            count1++;
            token = strtok(NULL," \t\n");
        }
        if(count1==2)
        {
            strcpy(op[c2].opc,sav[0].row);
            strcpy(op[c2].hex,sav[1].row);
        }
        c2++;

    }
    *val = c2;
    fclose(fp2);
}
void Display(struct input list[100],struct opdata op[100],int c1,int c2)
{
    FILE *fp3,*fp4,*fp5;
char l[20];
    char label[100];
    int addr,i,j,addr1,flag=0,value,count=0,k;
    struct check low[100];
    for(j=0;j<c2;j++)
        {
           printf("%s\t %s\n",op[j].opc,op[j].hex);
        }

    fp3 = fopen("SIC.txt","w");
    if(fp3==NULL)
    {
        printf("Unable to open file!");
    }
     fp4=fopen("Symboltable.txt","w");
     fp5=fopen("Symboltable.txt","r");
     if(!strcmp(list[1].opcode,"START"))
     {
         addr = 4096;
     }
    for(i=0;i<c1;i++)
    {
 strcpy(l,list[i].label);


           if(l[0]=='.')
           {
               fprintf(fp3,"%s\t%s\t%s\n",list[i].label,list[i].opcode,list[i].operand);

           }
           else
           {
                fprintf(fp3,"%x\t%s\t%s\t%s\n",addr,list[i].label,list[i].opcode,list[i].operand);
                printf("%x\t%s\t%s\t%s\n",addr,list[i].label,list[i].opcode,list[i].operand);
           }



        if(!strcmp(list[i].label,"") || !strcmp(list[i].label,".") )
        {

        }
        else
        {

//fprintf(fp4,"%x\t%s\t\n",addr,list[i].label);

         if(count==0)
         {
             low[count].addr = addr;
             strcpy(low[count].symbol,list[i].label);
             count++;
         }
         else
         {
             for(k=0;k<count;k++)
             {
                 if(!strcmp(low[k].symbol,list[i].label))
                 {
                     flag=1;
                 }

             }
             if(flag==1)
             {
                 printf("Duplicate Value!\n");
             }
             else
             {
                  low[count].addr = addr;
             strcpy(low[count].symbol,list[i].label);
             count++;
             }
             flag=0;
         }

        }






        for(j=0;j<c2;j++)
        {
            if(!strcmp(list[i].opcode,op[j].opc))
            {

                addr=addr+3;
            }
        }

        if(!strcmp(list[i].opcode,"WORD"))
        {
            addr=addr+3;
        }
        if(!strcmp(list[i].opcode,"RESW"))
        {
            value = atoi(list[i].operand);
            addr =addr+(value*3);
        }
        else if(!strcmp(list[i].opcode,"RESB"))
            {
                  value = atoi(list[i].operand);
                  addr = addr + value;
            }
        else if(!strcmp(list[i].opcode,"BYTE"))
                {
                    strcpy(l,list[i].operand);
                if(l[0]=='C')
                    {
                     value = strlen(list[i].operand);
                     addr = addr + (value-3);
                    }
                if(l[0]=='X')
                {
                     value = strlen(list[i].operand);
                     value = value-3;
                      addr = addr + (value/2);
                }
                }
                else
                {
                    addr = addr+0;
                }
    }


printf("The Total length is %x\n",addr-4096);
for(i=0;i<count;i++)
{
    fprintf(fp4,"%d %s\n",low[i].addr,low[i].symbol);
}


fclose(fp4);
fclose(fp3);
fclose(fp5);


}
int main()
{
    struct input list[100];
    struct str  sav[100];
    struct opdata op[100];
    char word[100];
    char *token,l[20],label[20];
    int c1=0,c2=0,value1,value2;


   readinput(list,sav,c1,&value1);

    readoptab(op,sav,c2,&value2);
    Display(list,op,value1,value2);

}
