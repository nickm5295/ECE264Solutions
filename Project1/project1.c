#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef struct FEL
{
    char action;
    float time;
    struct FEL * next;
    int priority;
    float serviceTime;
} FEL;

typedef struct ZeroC
{
    float timein;
    float serviceTime;
    struct ZeroC * next;
} ZeroC;

typedef struct ZeroQ
{
    struct ZeroC * head;
    struct ZeroC * tail;
    int numEl;
} ZeroQ;

typedef struct OneC
{
    float timein;
    float serviceTime;
    struct OneC * next;
} OneC;

typedef struct OneQ
{
    struct OneC * head;
    struct OneC * tail;
    int numEl;
} OneQ;

typedef struct Stats
{
    float waitTime0;
    float waitTime1;
    float QLength;
    float SUse;
    int total0;
    int total1;
} Stats;

void mode2(char* filename);
FEL * loadFEL(FILE * fp);
void performEvent(struct FEL ** fel, struct ZeroQ * zeroQ, struct OneQ * oneQ, int * s);
void addDeparture(FEL ** fel, int departureTime, int priority);
void addZeroC(FEL * fel, ZeroQ * zeroQ);
void addOneC(FEL * fel, OneQ * oneQ);
FEL * getSingleEvent(FILE * fp, int * working);
int getInt(FILE * fp, int * working);
int getDigit(FILE * fp);
void runSimulation(FEL * fel);
void mode1(int lamda0, int lamda1, int mu, int totalTasks);
int strToNum(char * str);
FEL * createFEL(int lamda, int mu, int totalTasks, int priority);
FEL * sortFELs(FEL ** fel0, FEL ** fel1);
void compAndSort(FEL ** target, FEL ** fel0, FEL ** fel1);
float createSingleFEL(FEL ** fel, int lamda, int mu, float time, int priority);

int main(int argc, char** argv)
{
    if(argc == 2)
    {
	mode2(argv[1]);
    }
    else if(argc == 5)
    {
	mode1(strToNum(argv[1]), strToNum(argv[2]), strToNum(argv[3]), strToNum(argv[4]));
    }
    return 1;
}

int strToNum(char * str)
{
    int strLeng = strlen(str), res = 0, loc = 0, digit;
    char ch;
    while(loc < strLeng)
    {
	ch = str[loc];
	digit = ch - '0';
	if(digit >= 0 && digit <= 9)
	{
	    res = res * 10 + digit;
	    loc++;
	}
	else loc = strLeng;
    }
    return res;
}

void mode1(int lamda0, int lamda1, int mu, int totalTasks)
{
    FEL * fel0 = createFEL(lamda0, mu, totalTasks, 0);
    FEL * fel1 = createFEL(lamda1, mu, totalTasks, 1);
    FEL * fel = sortFELs(&fel0, &fel1);
    runSimulation(fel);
}

FEL * sortFELs(FEL ** fel0, FEL ** fel1)
{
    FEL * res = NULL, * tail = NULL, *new = NULL;
    if(*fel0 != NULL && *fel1 != NULL)
    {
	compAndSort(&res, fel0, fel1);
	tail = res;
	while(*fel0 != NULL && *fel1 != NULL)
	{
	    compAndSort(&new, fel0, fel1);
	    tail->next = new;
	    tail = new;
	}
	if(*fel1 == NULL)
	{
	    tail->next = *fel0;
	}
	else
	{
	    tail->next = *fel1;
	}
    }
    else if(*fel1 == NULL)
    {
	return *fel0;
    }
    else
    {
	return *fel1;
    }
    return res;
}

void compAndSort(FEL ** target, FEL ** fel0, FEL ** fel1)
{
    if((*fel1)->time < (*fel0)->time)
    {
        *target = *fel1;
        *fel1 = (*fel1)->next;
    }
    else
    {
        *target = *fel0;
        *fel0 = (*fel0)->next;
    }
}

FEL * createFEL(int lamda, int mu, int totalTasks, int priority)
{
    FEL * res = NULL, * tail = NULL, * new = NULL;
    int i;
    float time = 0;
    if(totalTasks > 0)
    {
	time += createSingleFEL(&res, lamda, mu, time, priority);
	tail = res;	
    } 
    for(i = 1; i < totalTasks; i++)
    {
	time += createSingleFEL(&new, lamda, mu, time, priority);
	tail -> next = new;
	tail = new;
    }
    return res;
}

float createSingleFEL(FEL ** fel, int lamda, int mu, float time, int priority)
{
    float x = ((float) rand())/ RAND_MAX, r = (-1.0 / ((double) mu)) * log(1 - x);
    float interArrival = lamda * exp(-1 * lamda * r);
    float serviceTime = mu * exp(-1 * mu * r);
    *fel = malloc(sizeof(FEL));
    if(*fel != NULL)
    {
	(*fel)->action = 'a';
	(*fel)->time = time;
	(*fel)->next = NULL;
	(*fel)->priority = priority;
	(*fel)->serviceTime = serviceTime;
    }
    else printf("Error allocationg memory for FEL.-----\n");

    return interArrival;
} 

void mode2(char* filename)
{
    FILE * fp = NULL;
    fp = fopen(filename, "rb");
    if(fp != NULL)
    {
	FEL * fel = loadFEL(fp);
	fclose(fp);
	runSimulation(fel);
    }
}

void runSimulation(FEL * fel)
{
    float time = 0;
    float res;
    struct Stats stat;
    stat.waitTime0 = 0;
    stat.waitTime1 = 0;
    stat.QLength = 0;
    stat.SUse = 0;
    stat.total0 = 0;
    stat.total1 = 0;
    struct ZeroQ zeroQ;
    zeroQ.head = NULL;
    zeroQ.tail = NULL;
    zeroQ.numEl = 0;
    struct OneQ oneQ;
    oneQ.head = NULL;
    oneQ.tail = NULL;
    oneQ.numEl = 0;
    int s = 0;
    int interEventTimer;
    while(fel != NULL)
    {
	interEventTimer = fel->time - time;
	stat.QLength += (oneQ.numEl + zeroQ.numEl) * interEventTimer;
	stat.SUse += s * interEventTimer;
        time = fel->time;
	while(fel != NULL && fel->time == time)
        {
	    performEvent(&fel, &zeroQ, &oneQ, &s);
	}
        if(s == 0 && (zeroQ.numEl || oneQ.numEl))
	{
	    s = 1;
	    if(zeroQ.numEl)
	    {
	        stat.waitTime0+= time - (zeroQ.head)->timein;
	        addDeparture(&fel, time + (zeroQ.head)->serviceTime, 0);
	        ZeroC * oldC = zeroQ.head;
	        (zeroQ.head) = (zeroQ.head)->next;
	        zeroQ.numEl--;
	        free(oldC);
	        stat.total0++;
	    }
	    else
	    {
	        stat.waitTime1+= time - (oneQ.head)->timein;
	        addDeparture(&fel, time + (oneQ.head)->serviceTime, 1);
	        OneC * oldOneC = oneQ.head;
	        (oneQ.head) = (oneQ.head)->next;
	        oneQ.numEl--;
	        free(oldOneC);
	        stat.total1++;
	    }
	}
	//moment when all events happening at single point should have happened
    }
    if(stat.total0)
    {
	res = ((float) stat.waitTime0)/stat.total0;
    }
    else res = 0;
    printf("Avg wait time for 0's: %f\n", res);
    if(stat.total1)
    {
	res = ((float) stat.waitTime1)/stat.total1;
    }
    else res = 0;
    printf("Avg wait time for 1's: %f\n", res);
    if(time)
    {
	res = ((float) stat.QLength) / time;
    }
    else res = 0;
    printf("Avg Q length: %f\n", res);
    if(time)
    {
	res = ((float) stat.SUse) / time;
    }
    else res = 0;
    printf("Avg S use: %f\n", res);
}

void addDeparture(FEL ** fel, int departureTime, int priority)
{
    FEL * prev = *fel, * next = NULL;
    int done = 0;
    FEL * newFel = malloc(sizeof(newFel));
    newFel->action = 'd';
    newFel->time = departureTime;
    newFel->next = NULL;
    newFel->priority = priority;
    newFel->serviceTime = 0;
    if(*fel == NULL)
    {
	*fel = newFel;
	return;
    }
    if(prev->time > departureTime)
    {
	newFel->next = *fel;
	*fel = newFel;
	return;
    }
    while(!done)
    {
	if(prev->next!= NULL)
	{
	    next = prev->next;
	    if(next-> time < departureTime)
	    {
	    	prev = next;
	    }
	    else
	    {
	    	prev->next = newFel;
	    	newFel->next = next;
		done = TRUE;
	    }
	}
	else
	{
	    prev->next = newFel;
	    done = TRUE;
	}
    }
}

void performEvent(struct FEL ** fel, struct ZeroQ * zeroQ, struct OneQ * oneQ, int * s)
{
    if((*fel)->action == 'a')
    {
	(*fel)->priority == 0 ? addZeroC(*fel, zeroQ) : addOneC(*fel, oneQ);
    }
    else
    {
	*s = 0;
    }
    FEL * oldFel = *fel;
    *fel = (*fel)-> next;
    free(oldFel);
}

void addOneC(FEL * fel, OneQ * oneQ)
{
    OneC * oneC = malloc(sizeof(OneC));
    oneC-> timein = fel-> time;
    oneC-> serviceTime = fel-> serviceTime;
    oneC-> next = NULL;
    if(oneQ->head != NULL)
    {
    	(oneQ-> tail)-> next = oneC;
    }
    else oneQ->head = oneC;
    oneQ-> tail = oneC;
    (oneQ->numEl)++;
}

void addZeroC(FEL * fel, ZeroQ * zeroQ)
{
    ZeroC * zeroC = malloc(sizeof(ZeroC));
    zeroC-> timein = fel-> time;
    zeroC-> serviceTime = fel->serviceTime;
    zeroC-> next = NULL;
    if(zeroQ->head != NULL)
    {
    	(zeroQ-> tail)-> next = zeroC;
    }
    else zeroQ->head = zeroC;
    zeroQ-> tail = zeroC;
    (zeroQ->numEl)++;
} 

FEL * loadFEL(FILE * fp)
{
    int working = TRUE;
    FEL * fel = getSingleEvent(fp, &working), * lastFEL = fel, * new = NULL;
    while(working)
    {
	new = getSingleEvent(fp, &working);
	if(new != NULL)
	{
	    lastFEL->next = new;
	    lastFEL = new;
	}
    }
    
    return fel;
}

FEL * getSingleEvent(FILE * fp, int * working)
{
    int timeIn = getInt(fp, working);
    FEL * fel = NULL;
    if(timeIn >= 0)
    {
        int priority = getInt(fp, working);
        if(priority >= 0)
        {
            int serviceTime = getInt(fp, working);
            if(serviceTime >= 0)
            {
                fel = malloc(sizeof(FEL));
                if(fel != NULL)
                {
                    fel-> action = 'a';
                    fel-> time = timeIn;
                    fel-> next = NULL;
		    fel-> priority = priority;
		    fel-> serviceTime = serviceTime;
                }
                else
                {
                    printf("Error allocating memory for fel.\n");
                    *working = FALSE;
                }
            }
            else
            {
                *working = FALSE;
                printf("Error in file: wrong number of elements in line.\n");
            }
        }
        else
        {
            *working = FALSE;
            printf("Error in file: wrong number of elements in line.\n");
        }
    }
    else *working = FALSE;

    return fel;
}

int getInt(FILE * fp, int * working)
{
    int num = 0, done = FALSE, new, read = FALSE;
    while(done == FALSE)
    {
	new = getDigit(fp);
	if(new >= 0)
	{
	    num = num * 10 + new;
	    read = TRUE;
	}
	else
	{
	    done = TRUE;
	    if(new == -1) *working = FALSE;
	    if(read == FALSE) num = new;
	}
    }
    
    return num;
}

int getDigit(FILE * fp)
{
    int ch = fgetc(fp);
    if(ch == -1) return -1;
    int result = ch - '0';
    if(result < 0 || result > 9) result = -2;
    
    return result;
}
