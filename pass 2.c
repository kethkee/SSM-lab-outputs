#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main()
{
    FILE *intermediate,*symtab,*length,*objpgm,*asmlist,*optab;
    intermediate=fopen("intermediate.txt","r");
    symtab=fopen("symtab.txt","r");
    length=fopen("length.txt","r");
    optab=fopen("optab.txt","r");
    objpgm=fopen("objpgm.txt","w");
    asmlist=fopen("asmlist.txt","w");
    char label[20],opcode[20],operand[20],mnemonic[20],code[20],value[20],textrecd[70];
    int pgm_len,locctr,start_addr,text_addr,l=0;
    fscanf(intermediate,"%s%s%s",label,opcode,operand);
    fprintf(asmlist,"%s\t\t%s\t\t%s\n",label,opcode,operand);
    if(strcmp(opcode,"START")==0)
    {
        fscanf(length,"%d",&pgm_len);
        sscanf(operand,"%X",&start_addr);
        fprintf(objpgm,"H^%s^%06X^%06X\n",label,start_addr,pgm_len);
    }
    fscanf(intermediate,"%X%s%s%s",&locctr,label,opcode,operand);
    text_addr=locctr;
    strcpy(textrecd,"");
    while(strcmp(opcode,"END")!=0)
    {
        char objcode[20]="";
        fprintf(asmlist,"%04X\t\t%s\t\t%s\t\t%s",locctr,label,opcode,operand);
        if(strcmp(opcode,"BYTE")==0)
        {
            if(operand[0]=='C' && operand[1]=='\'')
            {
                for(int i=2;i<strlen(operand)-1;i++)
                {
                    sprintf(objcode+strlen(objcode),"%02X",operand[i]);
                }
            }
            else if(operand[0]=='X' && operand[1]=='\'')
            {
                for(int i=2;i<strlen(operand)-1;i++)
                {
                    sprintf(objcode+strlen(objcode),"%c",operand[i]);
                }
            }
        }
        else if(strcmp(opcode,"WORD")==0)
        {
            sprintf(objcode,"%06d",atoi(operand));
        }
        else if(strcmp(opcode,"RESW")==0)
        {}
        else if(strcmp(opcode,"RESB")==0)
        {}
        else{
            rewind(optab);
            while(fscanf(optab,"%s%s",mnemonic,code)!=EOF)
            {
                if(strcmp(opcode,mnemonic)==0)
                {
                    strcpy(objcode,code);
                    break;
                }
            }
            rewind(symtab);
            while(fscanf(symtab,"%s%s",label,value)!=EOF)
            {
                if(strcmp(operand,label)==0)
                {
                    int symbol_addr;
                    sscanf(value, "%X", &symbol_addr);      
                    sprintf(objcode + strlen(objcode), "%04X", symbol_addr); 

                    break;
                }
            }
        }
        fprintf(asmlist,"\t\t%s\n",objcode);
        if(l+strlen(objcode)>60)
        {
            fprintf(objpgm,"T^%06X^%02X^%s\n",text_addr,l/2,textrecd);
            text_addr=locctr;
            strcpy(textrecd,"");
            l=0;
        }
        if(strlen(objcode)>0)
        {
            strcat(textrecd,objcode);
            strcat(textrecd,"^");
            l+=strlen(objcode);
        }
        fscanf(intermediate,"%X%s%s%s",&locctr,label,opcode,operand);
    }
    if(l>0)
    {
        textrecd[strlen(textrecd)-1]='\0';
        fprintf(objpgm,"T^%06X^%02X^%s\n",text_addr,l/2,textrecd);
    }
    fprintf(objpgm,"E^%06X",start_addr);
    fclose(intermediate);
    fclose(symtab);
    fclose(optab);
    fclose(length);
    fclose(objpgm);
    fclose(asmlist);
}
