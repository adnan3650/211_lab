#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Artifact_struct {
    char name[20];
    int weight;
    int value;
} Artifact;

int totalWeight(Artifact* allArts, int numArts) {
    int tot = 0;
    for (int i = 0; i < numArts; ++i) {
        tot += allArts[i].weight;
    }
    return tot;
}

int totalValue(Artifact* allArts, int numArts) {
    int tot = 0;
    for (int i = 0; i < numArts; ++i) {
        tot += allArts[i].value;
    }
    return tot;
}

int maxValueFromHere(Artifact* allArts, int numArts, int here, int weightAllow) {
    if (here == numArts || weightAllow <= 0) {
        return 0;
    }
    if (allArts[here].weight > weightAllow) {
        return maxValueFromHere(allArts, numArts, here + 1, weightAllow);
    }
    int includeCurrent = allArts[here].value + maxValueFromHere(allArts, numArts, here + 1, weightAllow - allArts[here].weight);
    int excludeCurrent = maxValueFromHere(allArts, numArts, here + 1, weightAllow);
    return (includeCurrent > excludeCurrent) ? includeCurrent : excludeCurrent;
}

int maxValueFromStart(Artifact* allArts, int numArts, int weightAllow) {
    return maxValueFromHere(allArts, numArts, 0, weightAllow);
}

int maxValueFromHereFast(Artifact* allArts, int numArts, int here, int weightAllow, int** memo) {
    if (here == numArts || weightAllow <= 0) {
        return 0;
    }
    if (memo[here][weightAllow] != -1) {
        return memo[here][weightAllow];
    }
    if (allArts[here].weight > weightAllow) {
        memo[here][weightAllow] = maxValueFromHereFast(allArts, numArts, here + 1, weightAllow, memo);
    } else {
        int includeCurrent = allArts[here].value + maxValueFromHereFast(allArts, numArts, here + 1, weightAllow - allArts[here].weight, memo);
        int excludeCurrent = maxValueFromHereFast(allArts, numArts, here + 1, weightAllow, memo);
        memo[here][weightAllow] = (includeCurrent > excludeCurrent) ? includeCurrent : excludeCurrent;
    }
    return memo[here][weightAllow];
}

int maxValueFromStartFast(Artifact* allArts, int numArts, int weightAllow) {
    int** memo = (int**)malloc((numArts+1)*sizeof(int*));
    for (int i = 0; i < numArts+1; ++i ) {
        memo[i] = (int*)malloc((weightAllow+1)*sizeof(int));
        for (int j = 0; j < weightAllow+1; ++j ) {
            memo[i][j] = -1;
        }
    } 
    int res = maxValueFromHereFast( allArts, numArts, 0, weightAllow,memo);
    for (int i = 0; i < numArts+1; ++i) {
        free(memo[i]);
    }
    free(memo);
    return res; 
}

int readCollectionFile(int collectionNum, Artifact** allArtifacts) {
    int numArtifacts;
    char filename[20] = "collection";
    char appendThis[10];
    sprintf(appendThis,"%d.txt",collectionNum);
    strcat(filename,appendThis);

    FILE* myfile = fopen(filename,"r");
    if (myfile == NULL) {
        printf("File %s not found.\n",filename);
        return -1;
    }

    fscanf(myfile,"%d",&numArtifacts);
    *allArtifacts = (Artifact*)malloc(numArtifacts*sizeof(Artifact));
    for (int i = 0; i < numArtifacts; i++) {
        fscanf(myfile, "%s", (*allArtifacts)[i].name);
        fscanf(myfile, "%d", &(*allArtifacts)[i].weight);
        fscanf(myfile, "%d", &(*allArtifacts)[i].value);
    }
    fclose(myfile);

    return numArtifacts;
}

void printArtifacts(Artifact* allArts, int numArts) {
    for (int i = 0; i < numArts; ++i) {
        printf("  %s has weight = %d and value = %d\n",allArts[i].name,allArts[i].weight,allArts[i].value);
    }
}

int main() {
    int numArtifacts;
    int weightLimit;

    int collectionNum = 0; 
    while (collectionNum != 1 && collectionNum != 2 && collectionNum != 3 && collectionNum != 4) {
        printf("Enter the collection number (1, 2, 3, or 4):\n");
        scanf("%d",&collectionNum);
    } 
    
    printf("Enter the backpack weight limit:\n");
    scanf("%d",&weightLimit);

    Artifact* allArtifacts;
    numArtifacts = readCollectionFile(collectionNum, &allArtifacts);

    printf("Full list of artifacts:\n");
    printArtifacts(allArtifacts,numArtifacts);
    printf("Total weight of artifacts: %d\n",totalWeight(allArtifacts,numArtifacts));
    printf("Total value of artifacts: %d\n",totalValue(allArtifacts,numArtifacts));
    
    if (collectionNum == 1 || collectionNum == 2 || collectionNum == 3) {
        printf("Maximum possible value (slow mode) = $");
        printf("%d\n",maxValueFromStart( allArtifacts, numArtifacts, weightLimit));
    }  

    printf("Maximum possible value (fast mode) = $");
    printf("%d\n",maxValueFromStartFast( allArtifacts, numArtifacts, weightLimit));
 
    printf("Artifacts to put in backpack (weight capacity = %d): ??? \n", weightLimit);    

    free(allArtifacts);
    return 0;
}
