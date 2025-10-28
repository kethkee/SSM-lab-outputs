#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main()
{
    FILE *fp;
    int i,j,address;
    char name[10],line[100],addr[10];
    fp=fopen("input.txt","r");
    fscanf(fp,"%s",line);
    i=2;j=0;
    while(line[i]!='^' && line[i]!='\0')
    {
        name[j++]=line[i++];
    }
    name[j]='\0';
    printf("Program name is %s\n",name);
    do{
        fscanf(fp,"%s",line);
        if(line[0]=='T')
        {
            i=2;j=0;
            while(line[i]!='^' && line[i]!='\0')
            {
                addr[j++]=line[i++];
            }
            addr[j]='\0';
            address=(int)strtol(addr,NULL,16);
            int caret=0;
            i=0;
            while(line[i]!='\0' && caret<3)
            {
                if(line[i]=='^')
                {
                    caret++;
                }
                i++;
            }
            printf("Program loading from address %X\n",address);
            while(line[i]!='\0')
            {
                if(line[i]=='^')
                {
                    i++;
                    continue;
                }
                printf("%X\t\t%c%c\n",address,line[i],line[i+1]);
                i+=2;
                address++;
            }
        }
        else if(line[0]=='E')
        {
            printf("Program successfully loaded\n");
            break;
        }
    }while(!feof(fp));
    fclose(fp);
}
