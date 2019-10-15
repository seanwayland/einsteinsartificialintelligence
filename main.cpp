#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include<bits/stdc++.h>
#include<tuple>
#include"where.h"

#define POPSIZE 10
#define PARENTSIZE 4


using namespace std;

int population[POPSIZE][5][5]; // array to store the set of solns
int children[POPSIZE][5][5];
int fitness_scores[POPSIZE]; // array to store the "fitness" of each soln
int sorted_fitness_indexes[POPSIZE];
int counter = 0; // total iterations towards solution
int fitcount = 0; // counter to test fitness array
int MAXGEN = 500000; // total number of iterations
int parents[PARENTSIZE]; // array to store indexes of population chosen to be parents
int child[5][5]; // newborn infant !
int smallest_index = POPSIZE + 1;
int smallest = 0;
// these variables are the criteria which are tested
int C1 = 0;
int C2 = 0;
int C3 = 0;
int C4 = 0;
int C5 = 0;
int C6 = 0;
int C7 = 0;
int C8 = 0;
int C9 = 0;
int C10 = 0;
int C11 = 0;
int C12 = 0;
int C13 = 0;
int C14 = 0;
int C15 = 0;


int prints = 0; // set to one to print out condition results

int population_fitness = 0; // overall fitness of current population


// swaps an individual from one place in the population to another
void copy_individual(int source, int destination) {

    for (int j = 0; j < 5; j++) {
        for (int k = 0; k < 5; k++) {
            population[destination][j][k] = population[source][j][k];
        }
    }
}


//// THESE WHERE FUNCTION CHECK THE VARIOUS ATTRIBUTES
/// THEY SEARCH WITHIN AN INDIVIDUAL FOR THE LOCATION OF EACH ATTRIBUTE
//search in a row for an index
// no pink house ?
// check each house for a colour and return the index
int where_colour(int individual, int colour) {
    for (int i = 0; i < 5; i++) {
        if (population[individual][0][i] == colour) { return i; }
    }

}

// why doesn't this include aussies !!!
// check each house for a nationality and return the index
int where_nation(int individual, int nationality) {
    for (int i = 0; i < 5; i++) {
        if (population[individual][1][i] == nationality) { return i; }
    }

}

// plebian population
// no-one drinks wine
// look in all the houses and see who is pissed
int where_drink(int individual, int drink) {
    for (int i = 0; i < 5; i++) {
        if (population[individual][2][i] == drink) { return i; }
    }

}

// please don't smoke !
// if you smell dope in one of the houses call the police
int where_smoke(int individual, int smoke) {
    for (int i = 0; i < 5; i++) {
        if (population[individual][3][i] == smoke) { return i; }
    }

}

// I hope the pets get to run around outside sometimes!!
// if you spot an alligator in one of the houses run !!!
int where_pet(int individual, int pet) {
    for (int i = 0; i < 5; i++) {
        if (population[individual][4][i] == pet) { return i; }
    }

}

// Generates a random number within a certain range
// hopefully this returns a random number without too much bias!!

int random_number(unsigned long range) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, range); // distribution in range [1, 6]
    return int(dist6(rng));
}


// sorts a tuple in descending order
bool sortdesc(const tuple<int, int> &a,
              const tuple<int, int> &b) {
    return (get<0>(a) > get<0>(b));
}


// print out an individual from the population
void printIndividual(int individual) {
    cout << "INDIVIDUAL " << individual;
    for (int j = 0; j < 5; j++) {
        cout << " \n";
        for (int k = 0; k < 5; k++) {
            cout << population[individual][j][k] << " , ";
        }

    }
    cout << "\n";

}

// shuffle a designated individuals designated row in a designated individual
// not guaranteed to change row because a position might be swapped with itself
void shuffle_row(int individual, int row) {

    for (int i = 0; i < 5; i++) {
        int r = random_number(5) - 1;  // generate a random position
        int temp = population[individual][row][i];
        population[individual][row][i] = population[individual][row][r];
        population[individual][row][r] = temp;
    }


}

// mutate a selection all the members randomly
// amount is the number of mutations
// begin is used to protect individuals at start of population if needed
void mutate_population(int amount, int begin) {

    for (int aa = 0; aa < amount; aa++) {
        int indiv = random_number(POPSIZE);
        int roww = random_number(5);
        if (indiv > begin) {
            shuffle_row(indiv, roww);
        }
    }

}

/// GENERATE INDIVIDUAL POPULATION WITH RANDOMLY ARRANGED INTEGERS FROM 1 TO 5 IN 5 ROWS
// set all individuals to 1,2,3,4,5
void initialize_population() {
    fitcount = 0;
    for (int i = 0; i < POPSIZE; i++) {
        fitness_scores[i] = 0;
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                population[i][j][k] = k + 1;
            }
        }

    }

    // now shuffle them around a bit
    for (int ii = 0; ii < POPSIZE; ii++) {
        for (int kk = 0; kk < 5; kk++) {
            shuffle_row(ii, kk);
        }
    }
}


// tests an individual against the constraints
// a value of zero for everything is a win
// the individuals fitness is stored in the fitness array
// based on lecturers R code
void check_set_fitness(int individl) {
    int fit = 0;
    //   cout <<  " \n" ;
//constraints
//uses distances (how far away the constraints are from each other) instead of the obvious satisfied/not satisfied criteria
// makes the search a little smoother
//C1 = abs(which(house$color=="red") - which(house$nationality=="Brit"))
//The Brit lives in the red house
    C1 = abs(where_colour(individl, 3) - where_nation(individl, 2));
    if (prints == 1) { cout << "C1: " << C1 << " "; }
//C2 = abs(which(house$nationality=="Swede") - which(house$pet=="dogs"))
//# The Swede keeps dogs
    C2 = abs(where_nation(individl, 4) - where_pet(individl, 3));
//C3 = abs(which(house$nationality=="Dane") - which(house$drink=="tea"))
//# The Dane drinks tea
    if (prints == 1) { cout << "C2: " << C2 << " "; }
    C3 = abs(where_nation(individl, 1) - where_drink(individl, 4));
//C4 = abs(1 + which(house$color=="green") - which(house$color=="white"))
//# The green house is just to the left of the white one
    if (prints == 1) { cout << "C3: " << C3 << " "; }
    C4 = abs(1 + where_colour(individl, 2) - where_colour(individl, 4));
//C5 = abs(which(house$color=="green") - which(house$drink=="coffee"))
//# The owner of the green house drinks coffee
    if (prints == 1) { cout << "C4: " << C4 << " "; }
    C5 = abs(where_colour(individl, 2) - where_drink(individl, 2));
//C6 = abs(which(house$cigarette=="PallMall") - which(house$pet=="birds"))
//# The Pall Mall smoker keeps birds
    if (prints == 1) { cout << "C5: " << C5 << " "; }
    C6 = abs(where_smoke(individl, 4) - where_pet(individl, 1));
//C7 = abs(which(house$color=="yellow") - which(house$cigarette=="Dunhill"))
//# The owner of the yellow house smokes Dunhills
    if (prints == 1) { cout << "C6: " << C6 << " "; }
    C7 = abs(where_colour(individl, 5) - where_smoke(individl, 3));
    if (prints == 1) { cout << "C7: " << C7 << " "; }

//C8 = abs(which(house$drink=="milk") - 3)
//# The man in the center house drinks milk (2 is index of house in middle)
    C8 = abs(where_drink(individl, 3) - 2);
    if (prints == 1) { cout << "C8: " << C8 << " "; }
//C9 = abs(which(house$nationality=="Norwegian") - 1)
//# The Norwegian lives in the first house (1 is index of first house)
    C9 = (where_nation(individl, 5));
//if (prints == 1) {cout << "n" << where_nation(individl,5);}
//C10 = abs(1 - abs(which(house$cigarette=="Blend") - which(house$pet=="cats")))
//# The Blend smoker has a neighbor who keeps cats
    if (prints == 1) { cout << "C9: " << C9 << " "; }
    C10 = abs(1 - abs(where_smoke(individl, 1) - where_pet(individl, 2)));
//C11 = abs(1 - abs(which(house$pet=="horses") - which(house$cigarette=="Dunhill")))
//# The man who keeps horses lives next to the Dunhill smoker
    if (prints == 1) { cout << "C10: " << C10 << " "; }
    C11 = abs(1 - abs(where_pet(individl, 5) - where_smoke(individl, 3)));
//C12 = abs(which(house$drink=="beer") - which(house$cigarette=="BlueMaster"))
//# The man who smokes Blue Masters drinks beer
    if (prints == 1) { cout << "C11: " << C11 << " "; }
    C12 = abs(where_drink(individl, 1) - where_smoke(individl, 2));
//C13 = abs(which(house$nationality=="German") - which(house$cigarette=="Prince"))
//# The German smokes Prince
    if (prints == 1) { cout << "C12: " << C12 << " "; }
    C13 = abs(where_nation(individl, 3) - where_smoke(individl, 5));
//C14 = abs(1 - abs(which(house$nationality=="Norwegian") - which(house$color=="blue")))
//# The Norwegian lives next to the blue house
    if (prints == 1) { cout << "C13: " << C13 << " "; }
    C14 = abs(1 - abs(where_nation(individl, 5) - where_colour(individl, 1)));
//C15 = abs(1 - abs(which(house$cigarette=="Blend") - which(house$drink=="water")))
//# The Blend smoker has a neighbor who drinks water
    if (prints == 1) { cout << "C14: " << C14 << " "; }
    C15 = abs(1 - abs(where_smoke(individl, 1) - where_drink(individl, 5)));
    if (prints == 1) { cout << "C15: " << C15 << " "; }

//# fitness is simple sum of deviations from meeting constraints
//solution=cbind(C1,C2,C3,C4*10,C5*10,C6,C7,C8*10,C9*10,C10*10,C11*10,C12,C13,C14*10,C15*10)
//# puts extra weight on getting t order (colour) of houses right
//#solution=cbind(C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15) # no weights
//fit=sum(solution)
//return(fit)
    fit = C1 + C2 + C3 + (C4 * 10) + (C5 * 10) + C6 + C7 + (C8 * 10) + (C9 * 10) + (C10 * 10) + (C11 * 10) + C12 + C13 +
          (C14 * 10) + (C15 * 10);

    fitness_scores[individl] = fit; // stores all the fitness scores in an array

    C1 = C2 = C3 = C4 = C5 = C6 = C7 = C8 = C9 = C10 = C11 = C12 = C13 = C14 = C15 = 0;


}

// checks the fitness of each individual in the entire population
void check_all_scores() {
    // check the scores of entire population
    for (int d = 0; d < POPSIZE; d++) {
        check_set_fitness(d);
    }

    // sum the total fitness of population for testing purposes
    population_fitness = 0;
    for (int a = 0; a < POPSIZE; a++) {
        population_fitness = fitness_scores[a] + population_fitness;
    }
    cout << "\npopulation fitness: " << population_fitness;
    cout << "\n";
    smallest = fitness_scores[0];

    // check the array of scores for a WINNER
    for (fitcount = 0; fitcount < POPSIZE; fitcount++) {
        if (fitness_scores[fitcount] == 0) {
            cout << "\n SOLUTION FOUND\n";
            cout << "solution is at index: " << fitcount;
            cout << "\nCOUNTER IS: " << counter;
            cout << "\nWINNER IS: \n";
            printIndividual(fitcount);
            exit(0);

        }
        // checks for the smallest/best contender. for testing
        if (fitness_scores[fitcount] <= smallest) {
            smallest = fitness_scores[fitcount];
            smallest_index = fitcount;

        }


    }
    cout << "\nSmallest is: " << smallest;
    cout << "\nSmallest index is " << smallest_index;
    cout << "\n";
    // copy the best individual to the front of the array
    copy_individual(smallest_index, 0);
    fitcount = 0;


}


void copy_parents() {

    // create an array of tuples with an index and an amount
    vector<tuple<int, int>> v;
    for (int i = 0; i < POPSIZE; i++) {
        v.push_back(make_tuple(fitness_scores[i], i));
    }

    // Using sort() function to sort by 1st
    // element of tuple
    // generates an array of the fitness indexes sorted from high to low
    sort(v.begin(), v.end(), sortdesc);
    for (int j = 0; j < POPSIZE; j++) {
        sorted_fitness_indexes[j] = get<1>(v[j]);
    }
    // reverse so indexes are now low to high
    reverse(begin(sorted_fitness_indexes), end(sorted_fitness_indexes));

    // copy the best individual indexes into the parents array which stores their indexes in the population
    for (int jj = 0; jj < PARENTSIZE; jj++) {
        parents[jj] = sorted_fitness_indexes[jj];
    }


}

// for entire polution less first individual
// randomly select 2 individuals from parents
// copy to child
// copy child to population
void mate(int amount) {

    // repeat process number of mates we want
    for (int ll = 0; ll < amount; ll++) {

        // choose a random crossover point
        int crossover = random_number(5);
        // choose a random mum. LADIES FIRST
        int mum = parents[random_number(5)];
        // choose a random dad
        int dad = parents[random_number(5)];
        // mate by copying rows at randomly created crossover point
        for (int i = 0; i < crossover; i++) {
            for (int j = 0; j < 5; j++) {
                child[i][j] = population[mum][i][j];
            }
        }

        for (int ii = crossover; ii < 5; ii++) {
            for (int jj = 0; jj < 5; jj++) {
                child[ii][jj] = population[dad][ii][jj];
            }
        }
        // copy created child to children array
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                children[ll][j][k] = child[j][k];
            }
        }

    }

    // copy children to population leaving the first/best unchanged
    for (int dd = 1; dd < amount + 1; dd++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                population[dd][j][k] = children[dd][j][k];
            }
        }
    }
}


int main() {

    initialize_population();
    check_all_scores();
    for (counter = 0; counter < MAXGEN; counter++) {
        cout << "\n counter is now :" << counter;
        int mutate_amount = POPSIZE * 0.9;
        cout << "\n";
        copy_parents();
        mate(POPSIZE * 0.8);
        mutate_population(mutate_amount, 0);
        check_all_scores();
    }
    if (counter = MAXGEN) {
        cout << "\nresult not found. No of iterations is: " << MAXGEN;
    }

    return 0;

}


