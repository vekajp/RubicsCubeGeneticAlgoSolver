
#ifndef Genetic_hpp
#define Genetic_hpp

#include <stdio.h>
#include <vector>
#include "cubic.hpp"

class Individual {
private:
    double                              probability;
    int                                 genom_lenght;
    long long                           fitness_score;
    std::vector<std::pair<int, int>>    genes;
public:
    Individual();
    void empty();
    friend class Population;
    long long fitness(const Cube& cube);
    long long getFitness();
    std::vector<std::pair<int, int>>& getGenes();
    Cube& implement(Cube& cube);
    void mutate();
    friend long compare(Individual& a, Individual& b);
    //Individual crossover(Individual& parent);
};
class Population {
private:
    unsigned                    generations;
    long                        population_count;
    double                      average_fitness;
    Individual                  fittest;
    const Cube&                 cube;
    std::vector<Individual>     population;
    std::vector<Individual>     newPopulation;
public:
    bool found();
    Population(int n, const Cube& cube);
    void sortByFittest();
    void generateFirstPopulation(int n);
    void evoluteNewPopulation();
    void evoluate();
    void selection();
    void init();
    Individual& getFittest();
    long getBestScore();
    int getGenerations();
};
long compare(Individual& a, Individual& b);
#endif /* Genetic_hpp */
