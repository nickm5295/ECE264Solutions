#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "answer10.h"

#define TRUE 1
#define FALSE 0

typedef struct Zip
{
    char * zipName;
    char ** id;
    long int * fileLoc;
    long int numIds;
    struct Zip * left;
    struct Zip * right;
} Zip;

typedef struct State
{
    char * stateName;
    struct Zip * zip;
    struct State * left;
    struct State * right;
} State;

typedef struct Name
{
    char * name;
    struct State * state;
    struct Name * left;
    struct Name * right;
} Name;

typedef struct ReviewTree
{
    char * iD;
    long int * fileLoc;
    long int numReviews;
    struct ReviewTree * left;
    struct ReviewTree * right;
} ReviewTree;

typedef struct YelpDataBST
{
    char * businessesPath;
    char * reviewsPath;
    struct Name * nameHead;
    struct ReviewTree * reviewHead;
} YelpDataBST;

typedef struct HelperStruct 
{
    struct Location **locations;
    uint32_t * numLoc;
    char * state;
    struct State * stateHead;
    //struct Review * revHead;
    FILE * businessFp;
    FILE * reviewsFp;
    char * zip;
    struct Zip * zipHead;
    struct ReviewTree * reviewHead;
} HelperStruct;

typedef struct HelperStruct2
{
    FILE * reviewsFp;
    char * id;
    struct Review ** reviews;
    uint32_t * numReviews;
    struct ReviewTree * reviewHead;
} HelperStruct2;

typedef struct Business Business;
typedef struct Review Review;
typedef struct Location Location;

Name * loadNameFromFile(FILE * businessFp);
Name * nameInsert(Name * tail, Name * root);
void printBuffErr(char * buff);
void insertNameLeft(Name * node, Name * root);
void insertNameRight(Name * node, Name * root);
State * stateInsert(State * node, State * root);
void insertStateLeft(State * node, State * root);
void insertStateRight(State * node, State * root);
Zip * zipInsert(Zip * node, Zip * root);
void insertZipLeft(Zip * node, Zip * root);
void insertZipRight(Zip * node, Zip * root);
char * getString(FILE * fp);
State * loadStateFromFile(FILE * businessFp, char * iD);
Name * createNameNode(char * name, State * state);
int readThruStr(FILE * fp);
Zip * loadZipFromFile(FILE * businessFp, char * iD, long int fileLoc);
State * createStateNode(char * stateName, Zip * zip);
Zip * createZipNode(char * zipName, char * iD, long int fileLoc);
char * malStrUp(char * oStr);
ReviewTree * loadRevFromFile(FILE * reviewsFp);
ReviewTree * revInsert(ReviewTree * node, ReviewTree * root);
int readThruLine(FILE * fp);
ReviewTree * createRevNode(char * iD, long int fileLoc);
void insertReviewLeft(ReviewTree * node, ReviewTree * root);
void insertReviewRight(ReviewTree * node, ReviewTree * root);
struct Name* findName(char *name, struct Name * nameHead);
void allStates(struct HelperStruct helper);
void findState(HelperStruct helper);
ReviewTree * findReview(ReviewTree * revHead, char * id);
void addReview(HelperStruct2 helper2, long int numLoc);
void addLeftZips(struct HelperStruct helper);
void addZip(struct HelperStruct helper);
void addRightZips(struct HelperStruct helper);
void addLeftStates(struct HelperStruct helper);
void addState(HelperStruct helper);
void addRightStates(HelperStruct helper);
void freeRevTree(ReviewTree * revHead);
void freeName(struct Name * nameHead);
void freeState(State * stateHead);
void freeZip(Zip * zipHead);
void freeLocation(Location loc);
void freeRev(Review  rev);
char * newCopyUpp(char *oStr);

struct Business* get_business_reviews(struct YelpDataBST* bst, char *name, char *state, char *zip_code)
{
    int error = FALSE, i;
    Business * business = NULL;
    FILE * businessFp = NULL, * reviewsFp = NULL;
    Name * nameHead = NULL;
    ReviewTree * reviewHead = NULL;
    char * newName = malloc(sizeof(char) * (strlen(name) + 1));
    for(i = 0; i < strlen(name) + 1; i++)
    {
	newName[i] = name[i];
    }
    newName = malStrUp(newName);
    HelperStruct helper;
    if(bst == NULL || name == NULL) return NULL;
    businessFp = fopen(bst->businessesPath, "rb");
    reviewsFp = fopen(bst->reviewsPath, "rb");
    if(businessFp == NULL || reviewsFp == NULL) error = TRUE;
    if(error == FALSE)
    {
	nameHead = bst->nameHead;
	reviewHead = bst->reviewHead;
     	if(nameHead == NULL || reviewHead == NULL) error = TRUE;
    }
    if(error == FALSE)
    {
	nameHead = findName(newName, nameHead);
	if(nameHead == NULL) 
	{	
	    return NULL;
	}
	business = malloc(sizeof(Business));
	if(business == NULL)
  	{
	    fprintf(stderr, "Cannot allocate memory for business.\n");
	    return NULL;
	}
	reviewHead = bst->reviewHead;
	business->name = name;
	business->locations = NULL;
	business->num_locations = 0;
        helper.locations = &(business->locations);
	helper.numLoc = &(business->num_locations);
  	helper.state = state;
	helper.stateHead = nameHead->state;
	//helper.revHead = reviewHead;
	helper.businessFp = businessFp;
	helper.reviewsFp = reviewsFp;
	helper.zip = zip_code;
 	helper.zipHead = NULL;
      	helper.reviewHead = reviewHead;
	state == NULL ? allStates(helper) : findState(helper);
    }
    free(newName);
    return business;
}

void getReviews(struct HelperStruct2 helper2)
{
    helper2.reviewHead = findReview(helper2.reviewHead, helper2.id);
    long int i;
    for(i = 0; i < (helper2.reviewHead)->numReviews; i++)
    {
	addReview(helper2, i);
    }
}

ReviewTree * findReview(ReviewTree * revHead, char * id)
{
    ReviewTree * result = revHead;
    if(result == NULL) return NULL;
    int res = strcmp(id, result->iD);
    if(res < 0)
    {
	result = result->left;
	result = findReview(result, id);
    }
    else if(res > 0)
    {
	result = result->right;
	result = findReview(result, id);
    }
    /*else
    {
	result = helper2.reviewHead;
    }*/
    return result;
}

void addReview(HelperStruct2 helper2, long int numLoc)
{
    fseek(helper2.reviewsFp, ((helper2.reviewHead)->fileLoc)[numLoc], SEEK_SET);
    struct Review newRev, oldRev;
    char * rating = getString(helper2.reviewsFp);
    int i, stringLen = strlen(rating), score = 0, done = FALSE, numRevs, res; 
    char test, * newRevTxt = NULL, * oldRevTxt = NULL;
    for(i = 0; i < stringLen; i++)
    {
	test = rating[i];
	if(test >= '0' && test <= '9')
	{
	    score *= 10;
	    switch(test)
	    {
		case '1' : score+= 1; break;
		case '2' : score+= 2; break;
 		case '3' : score+= 3; break;
                case '4' : score+= 4; break;
		case '5' : score+= 5; break;
                case '6' : score+= 6; break;
                case '7' : score+= 7; break;
                case '8' : score+= 8; break;
		case '9' : score+= 9; break;
	    }
	}
    }
    free(rating);
    for(i = 1; i <= 3; i++)
    {
	readThruStr(helper2.reviewsFp);
    } 
    newRev.text = getString(helper2.reviewsFp);
    newRev.stars = score;
    numRevs = ++(*(helper2.numReviews));
    Review * revBuff = malloc(sizeof(Review) * numRevs);
    if(revBuff == NULL)
    {
        fprintf(stderr,"Failed to malloc review\n");
    }
    i = 0;
    while(i < numRevs - 1 && done == FALSE)
    {
	oldRev = (*(helper2.reviews))[i];
	if(newRev.stars < oldRev.stars)
  	{
	    revBuff[i] = oldRev;
	    i++;
	}
	else if(newRev.stars > oldRev.stars)
	{
	    done = TRUE;
	}
	else
	{
	    newRevTxt = newCopyUpp(newRev.text);
	    oldRevTxt = newCopyUpp(oldRev.text);
	    res = strcmp(newRevTxt, oldRevTxt);
	    free(newRevTxt);
	    free(oldRevTxt);
	    if(res >= 0)
	    {
		revBuff[i] = oldRev;
		i++;
	    }
	    else
	    {
		done = TRUE;
	    }
	}
    }
    revBuff[i] = newRev;
    for(++i; i < numRevs; i++)
    {
        revBuff[i] = (*(helper2.reviews))[i - 1];
    }
    free(*(helper2.reviews));
    (*(helper2.reviews)) = revBuff;
}

char * newCopyUpp(char *oStr)
{
    int i, strLeng = strlen(oStr) + 1;
    char * result = malloc(sizeof(char) * strLeng);
    for(i = 0; i < strLeng; i++)
    {
	result[i] = oStr[i];
    }
    result = malStrUp(result);
    return result; 
}

void addAddress(struct HelperStruct helper, long int numLoc)
{
    fseek(helper.businessFp, ((helper.zipHead)->fileLoc)[numLoc], SEEK_SET);
    struct Location newLoc, oldLoc;
    newLoc.address = getString(helper.businessFp);
    newLoc.city = getString(helper.businessFp);
    newLoc.state = strdup(helper.stateHead->stateName);
    newLoc.zip_code = strdup(helper.zipHead->zipName);
    newLoc.reviews = NULL;
    newLoc.num_reviews = 0;
    HelperStruct2 helper2;
    helper2.reviewsFp = helper.reviewsFp;
    helper2.id = ((helper.zipHead)->id)[numLoc];
    helper2.reviews = &(newLoc.reviews);
    helper2.numReviews = &(newLoc.num_reviews);
    helper2.reviewHead = helper.reviewHead;
    getReviews(helper2);
    int numLocs = ++(*(helper.numLoc));
    Location* locationBuff = malloc(sizeof(Location) * numLocs);
    if(locationBuff == NULL)
    {
	fprintf(stderr,"Failed to malloc locations\n");
    }
    int i = 0, done = FALSE, res;
    while(i < numLocs - 1 && done == FALSE)
    {
	oldLoc = (*(helper.locations))[i];
	res = strcmp(newLoc.city, oldLoc.city);
	if(res > 0)
	{
	    locationBuff[i] = oldLoc;
	    i++;
	}
	else if(res == 0)
    	{
	    res = strcmp(newLoc.address, oldLoc.address);
	    if(res > 0)
	    {
		locationBuff[i] = oldLoc;
		i++;
	    }
	    else if(res < 0)
	    {
		done = TRUE;
	    }
	    else
	    {
		fprintf(stderr, "Error, duplicate store!!\n");
	    }
	}
	else done = TRUE;
    } 
    locationBuff[i] = newLoc;
    for(++i; i < numLocs; i++)
    {
	locationBuff[i] = (*(helper.locations))[i - 1];
    }
    free(*(helper.locations));
    (*(helper.locations)) = locationBuff;
}

void findZip(HelperStruct helper)
{
    int res, done = FALSE;
    while(done == FALSE)
    {
	if(helper.zipHead == NULL) return;
   	res = strcmp(helper.zip, (helper.zipHead)->zipName);
	if(res < 0)
	{
	    helper.zipHead = (helper.zipHead)->left;
	}
	else if(res > 0)
	{
	    helper.zipHead = (helper.zipHead)->right;
	}
	else
	{
	    done = TRUE;
	}
    }
    addZip(helper);
}

void allZips(HelperStruct helper)
{
    if(helper.zipHead == NULL) return;
    addLeftZips(helper);
    addZip(helper);
    addRightZips(helper);
}

void allStates(HelperStruct helper)
{
    if(helper.stateHead == NULL) return;
    addLeftStates(helper);
    addState(helper);
    addRightStates(helper);
}

void findState(HelperStruct helper)
{
    int res, done = FALSE;
    while(done == FALSE)
    {
	if(helper.stateHead == NULL) return;
	res = strcmp(helper.state, (helper.stateHead)->stateName);
	if(res < 0)
 	{
	    helper.stateHead = (helper.stateHead)->left;
	}
	else if(res > 0)
	{
	    helper.stateHead = (helper.stateHead)->right;
	}
	else
	{
	    done = TRUE;
	}
    }
    addState(helper);
}

void addZip(struct HelperStruct helper)
{
    long int i;
    long int numIds = (helper.zipHead)->numIds;
    for(i = 0; i < numIds; i++)
    {
	addAddress(helper, i);
    }
}

void addState(HelperStruct helper)
{
    helper.zipHead = (helper.stateHead)->zip;
    helper.zip == NULL? allZips(helper) : findZip(helper);
}

void addRightZips(struct HelperStruct helper)
{
    helper.zipHead = (helper.zipHead)->right;
    allZips(helper);
}

void addRightStates(HelperStruct helper)
{
    helper.stateHead = (helper.stateHead)->right;
    allStates(helper);
}

void addLeftZips(struct HelperStruct helper)
{
    helper.zipHead = (helper.zipHead)->left;
    allZips(helper);
}

void addLeftStates(struct HelperStruct helper)
{
    helper.stateHead = (helper.stateHead)->left;
    allStates(helper);
}

struct Name* findName(char *name, struct Name * nameHead)
{
    int res = strcmp(name, nameHead->name);
    if(res < 0)
    {
	if(nameHead->left != NULL)
        {
	    return findName(name, nameHead->left);
    	}
    }
    else if(res > 0)
    {
	if(nameHead->right != NULL)
        {
	    return findName(name, nameHead->right);
	}
    }
    else
    {
	return nameHead;
    }
    return NULL;
}

struct YelpDataBST * create_business_bst(const char * businesses_path, const char * reviews_path)
{
    FILE * businessFp = NULL, * reviewsFp = NULL;
    YelpDataBST * bst = NULL;
    Name * nameHead = NULL,  * nameTail = NULL;
    ReviewTree * revHead = NULL, * revTail = NULL;   
 
    businessFp = fopen(businesses_path, "rb");
    if(businessFp != NULL)
    {
	reviewsFp = fopen(reviews_path, "rb");
	if(reviewsFp != NULL)
	{
 	    int done = FALSE;
	    do
	    { 
	    	nameTail = loadNameFromFile(businessFp);
		if(nameTail != NULL)
	    	{
		    nameHead = nameInsert(nameTail, nameHead);
	    	}
		else
		{
		    done = TRUE;
		}
	    } while(!done);
	    done = FALSE;
	    do
	    {
	 	revTail = loadRevFromFile(reviewsFp);
		if(revTail != NULL)
		{
		    revHead = revInsert(revTail, revHead);
		}
		else
		{
		    done = TRUE;
		}
	    } while(!done);
	    bst = malloc(sizeof(YelpDataBST));
	    int strLen = strlen(businesses_path);
	    char * buff = malloc(sizeof(char) * (strLen + 1));
	    printBuffErr(buff);
	    int i;
	    for(i = 0; i <= strLen; i++)
	    {
		buff[i] = businesses_path[i];
	    }
	    bst->businessesPath = buff;
	    strLen = strlen(reviews_path);
	    buff = malloc(sizeof(char) * (strLen + 1));
	    printBuffErr(buff);
	    for(i = 0; i <= strLen; i++)
            {
                buff[i] = reviews_path[i];
            }
	    bst->reviewsPath = buff;
	    bst->nameHead = nameHead;
	    bst->reviewHead = revHead;
	}
    }
    return bst;
}

ReviewTree * loadRevFromFile(FILE * reviewsFp)
{
    int err = FALSE;
    ReviewTree * revTr = NULL;
    char * iD = NULL;
    iD = getString(reviewsFp);
    if(iD == NULL)
    {
	err = TRUE;
    }
    long int fileLoc = ftell(reviewsFp);
    if(err == FALSE)
    {
    	if(!readThruLine(reviewsFp)) err = TRUE;
    }
    if(err == FALSE)
    {
	revTr = createRevNode(iD, fileLoc); 
    }
    return revTr;
}

ReviewTree * revInsert(ReviewTree * node, ReviewTree * root)
{
    if(root == NULL)
    {
	return node;
    }
    int res = strcmp(node->iD, root->iD);
    if(res < 0)
    {
	insertReviewLeft(node, root);
    }
    else if(res > 0)
    {
	insertReviewRight(node, root);
    }
    else
    {
  	(root->numReviews)++;
   	long int * buffLocs = malloc(sizeof(long int) * root->numReviews);
	if(buffLocs == NULL)
	{
	    fprintf(stderr, "Failed to allocate memory for location buffer.\n");
	}
	else
	{
	    int i;
	    for(i = 0; i < root->numReviews - 1; i++)
 	    {
		buffLocs[i] = (root->fileLoc)[i];
	    }
	    free(root->fileLoc);
	    buffLocs[i] = (node->fileLoc)[0];
	    root->fileLoc = buffLocs;
	    free(node->iD);
	    free(node->fileLoc);
	    free(node);
	}
    }
    return root;
}

Name * nameInsert(Name * node, Name * root)
{
    if(root == NULL)
    {
	return node;
    }
    int res = strcmp(node->name, root->name);
    if(res <0)
    {
	insertNameLeft(node, root);
    }
    else if(res > 0)
    {
	insertNameRight(node, root);
    }
    else
    {
	State * stateTail = node->state;
	free(node->name);
	free(node);
	stateInsert(stateTail, root->state);
    }
    return root;
}

State * stateInsert(State * node, State * root)
{
    if(root == NULL)
    {
	return node;
    }
    int res = strcmp(node->stateName, root->stateName);
    if(res < 0)
    {
	insertStateLeft(node, root);
    }
    else if(res > 0)
    {
	insertStateRight(node, root);
    }
    else
    {
	Zip * zipTail = node->zip;
    	free(node->stateName);
	free(node);
	zipInsert(zipTail, root->zip);
    }
    return root;
}

Zip * zipInsert(Zip * node, Zip * root)
{
    if(root == NULL)
    {
	return node;
    }
    int res = strcmp(node->zipName, root->zipName);
    if(res < 0)
    {
	insertZipLeft(node, root);
    }
    else if(res > 0)
    {
	insertZipRight(node, root);
    }
    else
    {
	int i;
	(root->numIds)++;
	char ** bufferId = malloc(sizeof(char *) * root->numIds);
	if(bufferId == NULL)
	{
	    fprintf(stderr, "Failed to allocate memory for bufferstr.\n");
	}
	for(i = 0; i < root->numIds - 1; i++)
	{
	    bufferId[i] = (root->id)[i];
	}
	free(root->id);
	bufferId[i] = (node->id)[0];
	root->id = bufferId;
	long int * fileBuff = malloc(sizeof(long int *) * root->numIds);
	if(fileBuff == NULL)
	{
	    fprintf(stderr, "Failed to allocate memory for fileBuff.\n");
	}
	for(i = 0; i < root->numIds - 1; i++)
	{
	    fileBuff[i] = (root->fileLoc)[i];
	}
	free(root->fileLoc);
	fileBuff[i] = (node->fileLoc)[0];
	root->fileLoc = fileBuff;
	
	free(node->zipName);
	free(node->id);
	free(node->fileLoc);
	free(node);
    }
    return root;
}

void insertReviewRight(ReviewTree * node, ReviewTree * root)
{
    if(root->right != NULL)
    {
	revInsert(node, root->right);
    }
    else
    {
	root->right = node;
    }
}

void insertReviewLeft(ReviewTree * node, ReviewTree * root)
{
    if(root->left != NULL)
    {
	revInsert(node, root->left);
    }
    else
    {
	root->left = node;
    }
}

void insertZipRight(Zip * node, Zip * root)
{
    if(root->right != NULL)
    {
	zipInsert(node, root->right);
    }
    else
    {
	root->right = node;
    }
}

void insertZipLeft(Zip * node, Zip * root)
{
    if(root->left != NULL)
    {
	zipInsert(node, root->left);
    }
    else
    {
	root->left = node;
    }
}

void insertStateRight(State * node, State * root)
{
    if(root->right != NULL)
    {
	stateInsert(node, root->right);
    }
    else
    {
	root->right = node;
    }
}

void insertStateLeft(State * node, State * root)
{
    if(root->left != NULL)
    {
	stateInsert(node, root->left);
    }
    else
    {
	root->left = node;
    }
}

void insertNameRight(Name * node, Name * root)
{
    if(root->right != NULL)
    {
	nameInsert(node, root->right);
    }
    else
    {
	root->right = node;
    }
}

void insertNameLeft(Name * node, Name * root)
{
    if(root->left != NULL)
    {
	nameInsert(node, root->left);
    }
    else
    {
	root->left = node;
    }
}

Name * loadNameFromFile(FILE * businessFp)
{
    int err = FALSE;
    Name * name = NULL;
    State * state = NULL;
    char * iD = NULL, * nameStr = NULL; 
    iD = getString(businessFp);
    if(iD == NULL)
    {
	err = TRUE;
    }
    if(err == FALSE)
    {
	nameStr = getString(businessFp);
	if(nameStr == NULL)
 	{
	    err = TRUE;
	}
    }
    if(err == FALSE)
    {
        state = loadStateFromFile(businessFp, iD);
    }
    if(state != NULL)
    {
  	name = createNameNode(nameStr, state);
    }
    return name;
}

State * loadStateFromFile(FILE * businessFp, char * iD)
{
    int err = FALSE;
    char * stateName = NULL;
    State * state = NULL;
    Zip * zip = NULL;
    long int fileLoc = ftell(businessFp);
    err = !(readThruStr(businessFp)); //really returns the address
    if(err == FALSE)
    {
	err = !(readThruStr(businessFp));
    }
    if(err == FALSE)
    {
	stateName = getString(businessFp); //now its really the state
      	if(stateName == NULL) err = TRUE;
    }
    if(err == FALSE)
    {
	zip = loadZipFromFile(businessFp, iD, fileLoc);
    }
    if(zip != NULL)
    {
	state = createStateNode(stateName, zip);
    }
    return state;
}

Zip * loadZipFromFile(FILE * businessFp, char * iD, long int fileLoc)
{
    //int correctRead;
    int err = FALSE;
    Zip * zip = NULL;
    char * zipName = NULL;
    zipName = getString(businessFp);
    /*if(zipName != NULL) 
    {
	correctRead = readThruStr(businessFp);
	if(correctRead != 1) err = TRUE;
    }
    if(err == FALSE)
    {
 	zip = createZipNode(zipName, iD, fileLoc);	
    }*/
    if(zipName == NULL) err = TRUE;
    if(err == FALSE) 
    {
	readThruStr(businessFp);
	zip = createZipNode(zipName, iD, fileLoc);
    }
    return zip;
}

Name * createNameNode(char * name, State * state)
{
    Name * node = malloc(sizeof(Name));
    node->name = malStrUp(name);
    node->state = state;
    node->left = NULL;
    node->right = NULL;
    return node;
}

State * createStateNode(char * stateName, Zip * zip)
{
    State * node = malloc(sizeof(State));
    node->stateName = malStrUp(stateName);
    node->zip = zip;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Zip * createZipNode(char * zipName, char * iD, long int fileLoc)
{
    Zip * zip = malloc(sizeof(Zip));
    zip->zipName = zipName;
    zip->id = malloc(sizeof(char*) * 1);
    (zip->id)[0] = iD;
    zip->fileLoc = malloc(sizeof(long int *) * 1);
    (zip->fileLoc)[0] = fileLoc;
    zip->numIds = 1;
    zip->left = NULL;
    zip->right = NULL;
    return zip;
}

ReviewTree * createRevNode(char * iD, long int fileLoc)
{
    ReviewTree * revTr = malloc(sizeof(ReviewTree));
    revTr->iD = iD;
    revTr->fileLoc = malloc(sizeof(long int *) * 1);
    (revTr->fileLoc)[0] = fileLoc;
    revTr->numReviews = 1;
    revTr->left = NULL;
    revTr->right = NULL;
    return revTr;
}

void freeRev(Review  rev)
{
    free(rev.text);
    //free(rev);
}

void freeLocation(Location  loc)
{
    free(loc.address);
    free(loc.city);
    free(loc.state);
    free(loc.zip_code);
    int i;
    for(i = loc.num_reviews - 1; i >= 0; i--)
    {
	freeRev((loc.reviews)[i]);
    }
    free(loc.reviews);
    //free(loc);
}

void destroy_business_result(struct Business* b)
{
    //free(b->name);
    int i;
    for(i = b->num_locations - 1; i >= 0; i--)
    {
	freeLocation((b->locations)[i]);
    }
    free(b->locations);
    free(b);
}

void freeZip(Zip * zipHead)
{
    if(zipHead == NULL) return;
    free(zipHead->zipName);
    long int i;
    for(i = zipHead->numIds - 1; i >= 0; i--)
    {
	free((zipHead->id)[i]);
    }
    free(zipHead->id);
    free(zipHead->fileLoc);
    freeZip(zipHead->left);
    freeZip(zipHead->right);
    free(zipHead);
}

void freeState(State * stateHead)
{
    if(stateHead == NULL) return;
    free(stateHead->stateName);
    freeZip(stateHead->zip);
    freeState(stateHead->left);
    freeState(stateHead->right);
    free(stateHead);
}

void freeName(struct Name * nameHead)
{
    if(nameHead == NULL) return;
    free(nameHead->name);
    freeState(nameHead->state);
    freeName(nameHead->left);
    freeName(nameHead->right);
    free(nameHead);
}

void freeRevTree(ReviewTree * revHead)
{
    if(revHead == NULL) return;
    free(revHead->iD);
    free(revHead->fileLoc);
    freeRevTree(revHead->left);
    freeRevTree(revHead->right);
    free(revHead);
}

void destroy_business_bst(struct YelpDataBST* bst)
{
    free(bst->businessesPath);
    free(bst->reviewsPath);
    freeName(bst->nameHead);
    freeRevTree(bst->reviewHead);
    free(bst);
}

int readThruLine(FILE * fp)
{
    int ch;
    int correct = 1, done = 0;
    do
    {
	ch = fgetc(fp);
     	switch(ch)
	{
	    case EOF:  correct = 0;
	    case '\n': done = 1; break;
	}
    } while(done == 0);
    return correct;
}

int readThruStr(FILE * fp)
{
    int ch;
    int correct = 1, done = 0;
    do
    {
	ch = fgetc(fp);
	switch(ch)
	{
	    case EOF:  correct = 0;
	    case '\t': 
	    case '\n': done = 1;  break;
	}
    } while(done == 0);
    return correct;
}

char * getString(FILE * fp)
{
    int size = 0, i, ch, done = FALSE, err = FALSE;
    char * result = malloc(sizeof(char));
    char * buffer = NULL;
    
    printBuffErr(result);
    while(done == FALSE)
    {
	ch = fgetc(fp);
	if(ch <= 255 && ch != EOF && ch >= 0)
	{
	    size++;
	    buffer = malloc(sizeof(char) * size);
	    printBuffErr(buffer);
	    for(i = 0; i < size - 1; i++)
	    {
	    	buffer[i] = result[i];
	    }
	    if(ch == '\t' || ch == '\n')
	    {
	    	ch = 0;
	    }
	    buffer[i] = (char) ch;
	    free(result);
	    result = buffer;
	    if(ch == '\0')
	    {
	    	done = TRUE;
	    }
	}
	else
	{
	    done = TRUE;
	    err = TRUE;
	}
    }
    if(err == FALSE)
    {
	return result;
    }
    free(result);
    return NULL;
}

char *  malStrUp(char * oStr)
{
    int strLen = strlen(oStr), i;
    char test;
    for(i = 0; i < strLen; i++)
    {
	test = oStr[i];
 	if(test >= 'a' && test <= 'z')
	{
	    oStr[i] -= 32;
	}
    }
    return oStr;
}

void printBuffErr(char * buff)
{
    if(buff == NULL)
    {
	fprintf(stderr, "Failed to allocate memory for buff.\n");
    }
}
