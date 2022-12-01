/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamprior <liamprior@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 02:47:06 by liamprior         #+#    #+#             */
/*   Updated: 2022/11/30 23:39:52 by liamprior        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define RED "\033[31m"
#define DEFAULT "\x1B[37m"
#define ERROR printf(RED "Please enter an input file! Program Exiting.\n")
#define MAX_NUM_SIZE 50//This wont need to be changed.
#define MAX_ARGS 9//going to be the registers, 0 is imm nums.
#define PASSCMD (variables.buffer = variables.buffer + 3)
#define ADD ((*variables.buffer == 'A' || *variables.buffer == 'a') && (*(variables.buffer + 1) == 'D' \
    || *(variables.buffer + 1) == 'd') && (*(variables.buffer + 2) == 'D' || *(variables.buffer + 2) == 'd') && (variables.choice = 1))//macrocs for parse, couldn have used hash map but too late.
#define AND ((*variables.buffer == 'A' || *variables.buffer == 'a') && (*(variables.buffer + 1) == 'N' \
    || *(variables.buffer + 1) == 'n') && (*(variables.buffer + 2) == 'D' || *(variables.buffer + 2) == 'd') && (variables.choice = 2))
#define ORR ((*variables.buffer == 'O' || *variables.buffer == 'o') && (*(variables.buffer + 1) == 'R' \
    || *(variables.buffer + 1) == 'r') && (*(variables.buffer + 2) == 'R' || *(variables.buffer + 2) == 'r') && (variables.choice = 3))
#define SUB ((*variables.buffer == 'S' || *variables.buffer == 's') && (*(variables.buffer + 1) == 'U' \
    || *(variables.buffer + 1) == 'u') && (*(variables.buffer + 2) == 'B' || *(variables.buffer + 2) == 'b') && (variables.choice = 4))
#define XOR ((*variables.buffer == 'X' || *variables.buffer == 'x') && (*(variables.buffer + 1) == 'O' \
    || *(variables.buffer + 1) == 'o') && (*(variables.buffer + 2) == 'R' || *(variables.buffer + 2) == 'r') && (variables.choice = 5))
#define NOT ((*variables.buffer == 'N' || *variables.buffer == 'n') && (*(variables.buffer + 1) == 'O' \
    || *(variables.buffer + 1) == 'o') && (*(variables.buffer + 2) == 'T' || *(variables.buffer + 2) == 't') && (variables.choice = 6))
#define LSR ((*variables.buffer == 'L' || *variables.buffer == 'l') && (*(variables.buffer + 1) == 'S' \
    || *(variables.buffer + 1) == 's') && (*(variables.buffer + 2) == 'R' || *(variables.buffer + 2) == 'r') && (variables.choice = 7))
#define LSL ((*variables.buffer == 'L' || *variables.buffer == 'l') && (*(variables.buffer + 1) == 'S' \
    || *(variables.buffer + 1) == 's') && (*(variables.buffer + 2) == 'L' || *(variables.buffer + 2) == 'l') && (variables.choice = 8))
#define ASR ((*variables.buffer == 'A' || *variables.buffer == 'a') && (*(variables.buffer + 1) == 'S' \
    || *(variables.buffer + 1) == 's') && (*(variables.buffer + 2) == 'R' || *(variables.buffer + 2) == 'r') && (variables.choice = 9))
#define MOV ((*variables.buffer == 'M' || *variables.buffer == 'm') && (*(variables.buffer + 1) == 'O' \
    || *(variables.buffer + 1) == 'o') && (*(variables.buffer + 2) == 'V' || *(variables.buffer + 2) == 'v') && (variables.choice = 10))



struct Variables {
    char *buffer, *ptr, numString[MAX_NUM_SIZE];//numString stores the hex strin and then loops again to the next num and stores that to.
    size_t size, lsize;
    int i, regNum, choice, RdReg, RegA, RegB;
    u_int32_t numNum[MAX_ARGS], answ; //change numNum to array and use x to iterate in for loop for future projects
    int32_t asrAnsw, asrNum;
    FILE* fp;
    bool of;
    long int res;
} Variables;

void RemoveNl(char *buffer) {
    int i = 0;
    if (*buffer != '\n') {//passing through the files empty lines
        while (buffer[i] != '\n' && buffer[i] != '\0')
            i++;
        buffer[i] = '\0';//removing new line
    }
}

void GatherHex(char **buffer, char *numString) {
    while (**buffer == ' ' || **buffer == '#')
        (*buffer)++;
    if (**buffer == '0' && *(*buffer + 1) == 'x' && (*buffer = *buffer + 2))//movoes buffer in if statement
        for (int d = 0; ((**buffer >= '0' && **buffer <= '9')
            || (**buffer >= 'A' && **buffer <= 'F')
                || (**buffer >= 'a' && **buffer <= 'f')); d++)//forloop for gathering only hex digits and not junk
                    numString[d] = *(*buffer)++;
    while (**buffer == ' ')
        (*buffer)++;
}

void GatherNum(char **buffer, char *numString) {
    while (**buffer == ' ' || **buffer == '#')
        (*buffer)++;
    for (int d = 0; **buffer >= '0' && **buffer <= '9'; d++)//forloop for gathering only digits and not junk
        numString[d] = *(*buffer)++;
    while (**buffer == ' ')
        (*buffer)++;
}

void ChangeToDec(int x, char *numString, u_int32_t numNum[MAX_ARGS]) {
    int base = 1;
    for(int y = (strlen(numString) - 1); y>=0; y--) {//converting hex string into dec uint32_t
        if(numString[y] >= '0' && numString[y] <= '9') {
            numNum[x] += (numString[y] - 48) * base;
            base *= 16;
        }
        else if(numString[y] >= 'A' && numString[y] <= 'F') {
            numNum[x] += (numString[y] - 55) * base;
            base *= 16;
        }
        else if(numString[y] >= 'a' && numString[y] <= 'f') {
            numNum[x] += (numString[y] - 87) * base;
            base *= 16;
        }
    }
}

void DecToHexString(int32_t numNum, char *numString) {//signed data type
    int temp = 0;
    if (numNum != 0 && numNum != -1) {//putting the total into hex form stored in a string (numString)
        for (int j = 0; (numNum != 0); j++) {//
            temp = numNum % 16;
            if (temp < 10)
                numString[j] = temp + 48;
            else
                numString[j] = temp + 55;
            numNum = numNum / 16;
        }
    } else if (numNum == -1) {
        for(int i=0;i<8;i++)
            numString[i] = 70;
    } else
        numString[0] = 48;
}

void U_DecToHexString(u_int32_t numNum, char *numString) {//unsigned data stype
    int temp = 0;
    // printf("mitch = %d\n", numNum);
    if (numNum != 0) {//putting the total into hex form stored in a string (numString)
        for (int j = 0; (numNum != 0); j++) {//
            temp = numNum % 16;
            if (temp < 10)
                numString[j] = temp + 48;
            else
                numString[j] = temp + 55;
            numNum = numNum / 16;
        }
    } else
        numString[0] = 48;
}

void PrintHex(bool of, char numString[MAX_NUM_SIZE]) {//printing output.
    int end = 0;
    while(numString[end])
        end++;
    while (end-- > 0)//printing backwards because above forloop converts the hex value into a string backwards for simplicity
        putchar(numString[end]);
    // printf("\n");
}

void initvariables(struct Variables *variables, char**argv){//use this to store variables
    variables->fp = fopen(*argv, "r");
    fseek(variables->fp, 0L, SEEK_END);
    variables->res = ftell(variables->fp);
    fseek(variables->fp, 0L, SEEK_SET);
    variables->of = 0;
    variables->answ = 0;
    variables->i = 0;
    variables->size = 32;
    variables->lsize = 0;
    variables->regNum = 0;
    variables->RegA = 0;
    variables->RegB = 0;
    variables->RdReg = 0;
    variables->choice = 0;// 1 add 2 and 3 orr
    variables->asrAnsw = 0;
}

int CheckOf(struct Variables *variables) {
    if(variables->answ < variables->numNum[0])
        variables->of=1;
    return 1;
}

void runOPP2(struct Variables *variables){
    if (variables->choice == 7)
        variables->numNum[variables->RdReg] = variables->numNum[variables->RegA] >> variables->numNum[8];
    else if (variables->choice == 8)
        variables->numNum[variables->RdReg] = variables->numNum[variables->RegA] << variables->numNum[8];
    else if (variables->choice == 9) {
        for (int i = 0; i < variables->numNum[8]; i++)
            variables->numNum[variables->RegA] = (int32_t)variables->numNum[variables->RegA] >> 1;
        variables->numNum[variables->RdReg] = variables->numNum[variables->RegA];
    }
}

void runOPP1(struct Variables *variables){
    if (variables->choice == 1 && CheckOf(variables))
        variables->numNum[variables->RdReg] \
        = variables->numNum[variables->RegA] \
        + variables->numNum[variables->RegB];
    else if (variables->choice == 2)
        variables->numNum[variables->RdReg] \
        = variables->numNum[variables->RegA] \
        & variables->numNum[variables->RegB];
    else if (variables->choice == 3)
        variables->numNum[variables->RdReg] \
        = variables->numNum[variables->RegA] \
        | variables->numNum[variables->RegB];
    else if (variables->choice == 4)
        variables->numNum[variables->RdReg] \
        = variables->numNum[variables->RegA] \
        - variables->numNum[variables->RegB];
    else if (variables->choice == 5)
        variables->numNum[variables->RdReg] \
        = variables->numNum[variables->RegA] \
        ^ variables->numNum[variables->RegB];
}

void getRegister(struct Variables *variables) {//grabs the reg number
    while (*variables->buffer=='r'||*variables->buffer=='R'||*variables->buffer==' ')
        variables->buffer++;
    variables->regNum = *variables->buffer - 48;
    variables->buffer += 2;//pushes past comma
}

void printRegs(struct Variables variables) {
    for (int i = 0; i < 8; i++) {
        bzero(variables.numString, MAX_NUM_SIZE);
        U_DecToHexString(variables.numNum[i], variables.numString);
        printf("R%d:0X",i);
        PrintHex(0, variables.numString);
        if (i == 3) printf("\n");
        else printf("  ");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    struct Variables variables;
    if ((*(argv + 1) == NULL) && ERROR)
        return 0;
    argv = argv + 1;
    initvariables(&variables, argv);
    variables.ptr = (char *)malloc(variables.size);//may need to check this, esp on there machines but getline reallocs
    variables.buffer = variables.ptr;
    bzero(variables.buffer, variables.res);
    bzero(variables.numString, MAX_NUM_SIZE);
    for (int i = 0; i < MAX_ARGS; i++)
        variables.numNum[i] = 0;
    while ((variables.lsize = getline(&variables.buffer, &variables.size, variables.fp)) != -1) {//puts the line from the file into buffer.
        while (*variables.buffer == ' ')
            variables.buffer++;
        variables.i = 0;
        RemoveNl(variables.buffer);
        printf("%s\n", variables.buffer);
        bzero(variables.numString, MAX_NUM_SIZE);//*****after this line i make if to see if its immediate or register driven
        if ((ADD || AND || ORR || SUB || XOR) && PASSCMD) {//needs to be able to take in registers, currently only works with immidate values. I can do a pre parse on each line and if i find an R or r then i know to use a boolean
            getRegister(&variables);
            variables.RdReg = variables.regNum;
            getRegister(&variables);
            variables.RegA = variables.regNum;
            getRegister(&variables);
            variables.RegB = variables.regNum;
            runOPP1(&variables);
        } else if (NOT && PASSCMD && printf("NOT: ")) {//needs to be able to take in registers
            printf("WASNT IN THE PROMT\n");
            exit (0);
        }
        else if ((LSL || LSR || ASR) && PASSCMD) {//needs to be able to take in registers, that means i cant use gather hex, need to parse everything first
            getRegister(&variables);
            variables.RdReg = variables.regNum;
            getRegister(&variables);
            variables.RegA = variables.regNum;
            GatherNum(&variables.buffer,variables.numString);
            ChangeToDec(8, variables.numString, variables.numNum);
            runOPP2(&variables);
        } else if (MOV && PASSCMD) {
            getRegister(&variables);
            GatherHex(&variables.buffer,variables.numString);
            ChangeToDec(8, variables.numString, variables.numNum);
            variables.numNum[variables.regNum] = variables.numNum[8];//[8] is the imm value
        }
        variables.numNum[8] = 0;
        bzero(variables.numString, MAX_NUM_SIZE);
        printRegs(variables);
    }
    free(variables.ptr);
    fclose(variables.fp);
}
