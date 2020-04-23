#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#define LENGTH sizeof(TEAM)/sizeof(TEAM[0])
#define GROUPNUM sizeof(GROUP)/sizeof(GROUP[0])


char *TEAM[] = {"Ajax", "Atalanta", "Atletico", "Barcelona", "Bayern",
                "Benfica", "Brugge", "Chelsea", "Crvenazvezda", "Dortmund", "Galatasaray",
                "Genk", "Internazionale", "Juventus", "Leipzig", "Leverkusen", "Liverpool",
                "Lokomotiv", "Lille", "Lyon", "ManCity", "Napoli", "Olympiacos", "Paris",
                "RealMadrid", "Salzburg", "Shakhtar", "SlaviaPraha", "Tottenham", "Valencia",
                "Zagreb", "Zenit", "Arsenal"};


char *COUNTRY[] = {"Netherlands", "Italy", "Spain", "Spain", "Germany",
                   "Portugal", "Belgium", "England", "Serbia", "Germany", "Turkey", "Belgium",
                   "Italy", "Italy", "Germany", "Germany", "England", "Russia", "France",
                   "France", "England", "Italy", "Greece", "France", "Spain", "Austria",
                   "Ukraine", "Czech", "England", "Spain", "Croatia", "Russia", "England"};

char *GROUP[] ={"group A", "group B", "group C", "group D", "group E", "group F", "group G", "group H"};


int radNum(int seed){
    srand(seed);
    return rand()%8;
}


const char* getCountry(char * team){
    int i;
    char* str;
    for(i=0;i<LENGTH;i++){
        str = TEAM[i];
        if ( ( strcmp(team, str) ) == 0){
            break;
        }
    }

    return COUNTRY[i];
}


int canTheyMeet(char* str1, char* str2){
    str1 = getCountry(str1);
    str2 = getCountry(str2);
    int flag=1;
    if ( (strcmp(str1, str2)) == 0 )
        flag = 0;
    if ( (strcmp(str1, "Ukraine")) == 0 && (strcmp(str2, "Russia")) == 0 )
        flag = 0;

    if ( (strcmp(str2, "Ukraine")) == 0 && (strcmp(str1, "Russia")) == 0 )
        flag = 0;

    return flag;
}




// Return Country and It cannot be changed

int whichPot(char * team, char *** result){
    int i,j,flag=0;
    for(i=0;i<4;i++){
        for(j=0;j<8;j++){
            char *str = result[i][j];
            if( (strcmp(team, str)) == 0 ){
                flag=1;
                break;
            }
        }
        if(flag==1)
            break;
    }

    return i+1;
}

int isGroupValid(char ** group, char *** result){
    int i,j;
    int flag = 1;
    for(i=0;i<3;i++){
        for(j=i+1;j<4;j++){
            char *str1 = group[i];
            char *str2 = group[j];
            if(!canTheyMeet(str1, str2) || whichPot(str1, result) == whichPot(str2, result)){
                flag=0;
                break;
            }
        }
    }
    return flag;
}




// only for test mode, we generate the Group for each teams
char *** generateGroup(char** argv, int length){
    int m;
    char *** result;
    result = (char ***)malloc(sizeof(char**)*8);
    for(m=0;m<8;m++){
        result[m] = (char**)malloc(sizeof(char*)*4);
    }
    int k,j;
    int i=35;
    for(k=0;k<8;k++){
        for(j=0;j<4;j++){
            char* string = argv[i + j];

            result[k][j] = string;
        }
        i+=4;
    }

    return result;

}


// set Teams to different pots
// The pots are three dimensional Array
char *** getTeams(char** argv, int length){
    int m;
    char *** result;
    result = (char ***)malloc(sizeof(char**)*4);
    for (m=0;m<4;m++){
        result[m] = (char**)malloc(sizeof(char*)*8);
    }


    int i=3;
    int j=0;
    int k=0;
     //Iterative get all the information of setmode
    for(; k<4; k++) {
        // four pots

            for (; j < 8; j ++) {
                // get string
                char *string = argv[i + j];
                result[k][j] = string;
            }
            i+=8;
            j=0;
    }
    return result;
}



void TestMode(char ** argv, int length, int id){
//    char buffer[16000];
//    setvbuf(stdout, buffer, _IOFBF, sizeof (buffer));
    char *** teamPot = getTeams(argv, length);
    char *** groups = generateGroup(argv, length);
    int i,j,validBit=1;
    for(i=0;i<4;i++){
        printf("Child %d , pid %d : teams for pot %d are : ", id, getpid(), i);



        for(j=0;j<8;j++){
            printf("%s ", teamPot[i][j]);

        }
        printf("\n");
        sleep(1);

    }

    for(i=0;i<8;i++){
        printf("Child %d , pid %d : Teams for %s are ", id, getpid(), GROUP[i]);


        for(j=0;j<4;j++){
            printf("%s ", groups[i][j]);

        }
        printf("\n");
        sleep(1);

        printf("Child %d , pid %d : pots for %s are ", id, getpid() ,GROUP[i]);


        for(j=0;j<4;j++){
            printf("%d ", whichPot(groups[i][j], teamPot));


        }
        printf("\n");
        sleep(1);

        printf("Child %d , pid %d : country for %s are ", id, getpid() ,GROUP[i]);


        for(j=0;j<4;j++){
            printf("%s ", getCountry(groups[i][j]));


        }
        printf("\n");
        sleep(1);
        char ** group = groups[i];
        if(!isGroupValid(group, teamPot)){
            printf("Child %d, pid %d : InValid Grouping\n" , id, getpid());
            sleep(1);
            validBit=0;
            break;
        }
    }
    if(validBit) {
        printf("Child %d, pid %d : Valid Grouping\n", id, getpid());
        sleep(1);
    }
    free(teamPot);
    free(groups);



}

int isIn(int t, const int* arr, int length){
    int j;
    for(j=0;j<length;j++){
        if (arr[j] == t){
            return 1;
        }
    }
    return 0;

}

int addToArray(int t, int * arr, int cap, int maxi){

    if (cap >= maxi){
        return -1;
    }
    arr[cap] = t;
    cap += 1;
    return cap;
}

void setMinusArray(int * arr, int length){
    int i;
    for(i=0;i<length;i++){
        arr[i] = -1;
    }

}

void GenerateMode(char** argv, int argc, int id, int theIndex){

    char *** teamPot = getTeams(argv, argc);

    int times = atoi(argv[theIndex]);
    int seed = atoi(argv[theIndex+1]);
    srand(seed);

    // generate Groups
    int m;
    char *** result;
    int totalRound = times;
    int trueRound=totalRound;
    result = (char ***)malloc(sizeof(char**)*8);
    for(m=0;m<8;m++){
        result[m] = (char**)malloc(sizeof(char*)*4);
    }


    int i, j;
    for (i = 0; i < 4; i++) {
        printf("Child %d , pid %d : teams for pot %d are : ", id, getpid(), i);

        for (j = 0; j < 8; j++) {
            printf("%s ", teamPot[i][j]);

        }
        printf("\n");
        sleep(1);
    }

    int index;
    for(index=0;index<times;index++) {



        int another_rad;

        for (i = 0; i < 4; i++) {

            int rad_num = rand() % 8;

            int ra[8];
            setMinusArray(ra, 8);

            int cap = 0;
            cap = addToArray(rad_num, ra, cap, 7);

            for (j = 0; j < 8; j++) {
                another_rad = rand() % 8;

                while (isIn(another_rad, ra, 8) == 1) {
                    another_rad = rand() % 8;
                }
                cap = addToArray(another_rad, ra, cap, 7);
            }

            while (isIn(another_rad, ra, 8) == 1) {
                another_rad = rand() % 8;
            }

            ra[7] = another_rad;




            for (j = 0; j < 8; j++) {
                char *team = teamPot[i][j];

                result[ra[j]][i] = team;

            }


        }


        for (j = 0; j < 8; j++) {
            char **group = result[j];
            if (!isGroupValid(group, teamPot)) {
                trueRound--;
                break;
            }
        }



    }



    printf("Child %d, pid %d : Seed %d with %d valid drawings out of %d (%.2f%%)\n", id, getpid(), seed, trueRound, totalRound, (double)trueRound/totalRound*100);
    sleep(1);

    free(result);
    free(teamPot);


}

void EvaluateMode(char** argv, int argc, int id, int theIndex, int lastTwoIndex){
    char *** teamPot = getTeams(argv, argc);

    int times = atoi(argv[theIndex]);
    int seed = atoi(argv[theIndex+1]);
    srand(seed);

    char *str1 = argv[lastTwoIndex];
    char *str2 = argv[lastTwoIndex+1];
    // generate Groups
    int m;
    char *** result;
    int totalRound = times;
    int validRound  = totalRound;
    int trueRound = 0;
    result = (char ***)malloc(sizeof(char**)*8);
    for(m=0;m<8;m++){
        result[m] = (char**)malloc(sizeof(char*)*4);
    }
    int i, j;
    for (i = 0; i < 4; i++) {
        printf("Child %d , pid %d : teams for pot %d are : ", id, getpid(), i);

        for (j = 0; j < 8; j++) {
            printf("%s ", teamPot[i][j]);

        }
        printf("\n");
        sleep(1);
    }

    int index;
    for(index=0;index<times;index++) {



        int another_rad;

        for (i = 0; i < 4; i++) {

            int rad_num = rand() % 8;

            int ra[8];
            setMinusArray(ra, 8);

            int cap = 0;
            cap = addToArray(rad_num, ra, cap, 7);

            for (j = 0; j < 8; j++) {
                another_rad = rand() % 8;

                while (isIn(another_rad, ra, 8) == 1) {
                    another_rad = rand() % 8;
                }
                cap = addToArray(another_rad, ra, cap, 7);
            }

            while (isIn(another_rad, ra, 8) == 1) {
                another_rad = rand() % 8;
            }

            ra[7] = another_rad;




            for (j = 0; j < 8; j++) {
                char *team = teamPot[i][j];

                result[ra[j]][i] = team;

            }


        }


        int k;
        int flag=1;
        for (j = 0; j < 8; j++) {
            char **group = result[j];
            if (!isGroupValid(group, teamPot)) {
                validRound--;
                flag=0;
                break;
            }
        }

        if(flag == 1) {

            for (j = 0; j < 8; j++) {
                char **group = result[j];
                if (isGroupValid(group, teamPot)) {


                    for (i = 0; i < 3; i++) {

                        for (k = i + 1; k < 4; k++) {
                            char *g1 = result[j][i];
                            char *g2 = result[j][k];
                            if (((strcmp(str1, g1)) == 0 && (strcmp(str2, g2)) == 0) ||
                                ((strcmp(str1, g2)) == 0 && (strcmp(str2, g1)) == 0)) {
                                trueRound++;
                                break;
                            }
                        }
                    }
                }
            }
        }



    }

    printf("Child %d, pid %d : seed %d (%d) %s meet %s %d draw from %d valid (%.2f%%) \n", id, getpid(), seed, times, str1, str2, trueRound, validRound, (
    double)trueRound/validRound *100 );
    sleep(1);

    free(teamPot);
    free(result);

}




int main(int argc, char **argv) {


    int k=0;




    // get number of child process
    int numOfChild = atoi(argv[1]);
    // The parent Id
    pid_t ppid = getpid();

    printf("Parent, pid %d : %d children %s Mode \n", ppid, numOfChild, argv[2]);
    pid_t pid;

    for(k=0;k<numOfChild;k++){
        if(fork() == 0){
            if ((strcmp(argv[2], "T") == 0)){
                TestMode(argv, argc, k+1);
                exit(0);
            }else if ((strcmp(argv[2], "G")) == 0){
                GenerateMode(argv, argc, k+1, 35+k*2);
                exit(0);
            }else if((strcmp(argv[2], "E")) == 0){
                EvaluateMode(argv, argc, k+1, 35+k*2, 35+numOfChild*2);
                exit(0);
            } else{
                printf("Wrong Operation");
                exit(0);
            }

        }
    }
    for(k=0;k<numOfChild;k++){
        wait(NULL);
    }


    return 0;
}













