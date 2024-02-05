#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "data.h"
#include "./structs/dictionary.h"

/*
	this function converts decimal numbers 
	to their binary form. 
	also takes care for negtive number with the 
	2 complaminet mothed
*/
char* decToBin(int num, int limit) {
	int i=0,j=0;
	unsigned uNum;
	char* temp = (char*)calloc(33,sizeof(char)); 
	char* bin = (char*)calloc(33,sizeof(char)); 
	uNum = (unsigned int)num;
	if (uNum == 0) {
		strcpy(bin, "0");
		free(temp);
		return bin;
	}
	
	for(;uNum;i++){
		temp[i] ='0' + (uNum & 01); 
		uNum >>= 1;
	}
	bin[i--] = '\0';
	for(j=0; i >= 0; j++, i--){
		bin[j] = temp[i];
	}
	if (limit && num < 0)
		bin = bin + 32 - limit;
	free(temp);
	return bin;
}
/* used to convers binary represantion of a number back to his decimal value */
int binToDec(char* bin) {
	int i=0, num=0;
	int j = strlen(bin) - 1;
	for(; j>=0; j--){ 
		num += ((bin[j] - '0') * pow(2,i));
		i++;
	}
	return num;
}
/*
	taken a 5 length binary string gets his decimal value
	and return the right symbol int the 32 base 
*/
char bin5ToBase32(char *num) {
	int x = binToDec(num);
	char symbols[32] = {'!', '@', '#', '$', '%', '^', '&', '*',
						'<', '>', 'a', 'b', 'c', 'd', 'e', 'f',
						'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
						'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};
	return symbols[x];			
}

/*
	takes a binary reprsentation of a string 
	checks if the lenght is divisable by 5 if not adds zero at start 
	then takes 5 charcters every  time and converts it to it base 32 
*/
char* binToBase32(char *bin) {
	char *num, *betterBin, *result;
	int i, len;
	num = (char *)calloc(6, sizeof(char));
	result = (char *)calloc(6, sizeof(char));
	len = strlen(bin);
	if (len % 5) {
		betterBin = (char *)calloc((((len / 5) + 1 )* 5 )+ 1, sizeof(char));
		for (i = 0; i < 5 - (len % 5); i++) 
			betterBin[i] = '0';
		strcat(betterBin, bin);
	}
	else {
		betterBin = bin;
	}
	i = 0;
	while (betterBin[0] != '\0') {
		strncpy(num, betterBin, 5);
		betterBin += 5;
		result[i++] = bin5ToBase32(num); 
	}
	return result;
}
/* converts decimal number to base 32 */
char* decToBase32(int x) {
	char *bin = decToBin(x, 0);
	char *res = binToBase32(bin);
	free(bin);
	return res;
}
