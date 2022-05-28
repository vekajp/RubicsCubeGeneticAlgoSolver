

#include "genetic.hpp"
std::vector<std::string> mutations = {
    "FRBLULiUBiRiFiLiUiLUi",
    "FiLiBiRiUiRUiBLFRURiU",
    "LDiLiFiDiFUFiDFLDLiUi",
    "RiDRFDFiUiFDiFiRiDiRU",
    "UF2UiRiDiLiF2LDR",
    "UiF2ULDRF2RiDiLi",
    "RiURUiRiUFRBiRBRFiR2",
    "LUiLiULUiFiLiBLiBiLiFL2",
    "FiUBUiFUBiUi",
    "FUiBiUFiUiBU",
    "RLiU2RiLF2",
    "LiRU2LRiF2",
    "RURiUi",
    "LiUiLU",
    "URURiUBiLiBUL",
    "UiLiUiLU",
    "RURiUi",
    "FRURiUiFi",
    "F",
    "Fi",
    "F2"
};
//think about replacement
std::vector<plane> planesinfront = {
    {0, 5, 3, 2, 1, 4},
    {1, 0, 2, 4, 5, 3},
    {2, 0, 3, 4, 1, 5},
    {3, 0, 5, 4, 2, 1},
    {4, 2, 3, 5, 1, 0},
    {5, 4, 3, 0, 1, 2}};

Individual::Individual() {
    fitness_score = -1;  //start fitness indicates solution is not valued
    genom_lenght = 5;   //start soltion len
    genes.resize(genom_lenght);
    
    //generating random solution
    srand(static_cast<unsigned int>(time(nullptr)));
    std::generate(genes.begin(), genes.end(), [](){return std::pair<int, int>({rand() % 6, rand() % 3 + 1});});
}

//enpty solution always present in the set
void Individual::empty() {
    genom_lenght = 0;
}

long long Individual::fitness(const Cube& cuberef) {
    fitness_score = 0;
    Cube cube = cuberef;
    cube = implement(cube);                         //implementing solution on start cube
    fitness_score += cube.countWrongFaceLets();     //plane pieces
    fitness_score += cube.countWrongEdges();        //edges
    return fitness_score;
}

Cube& Individual::implement(Cube& cube) {               //simply rotating planes
    for (int i = 0; i < genom_lenght; i++) {
        cube.rotatePlane(genes[i].first, genes[i].second);
    }
    return cube;
}
// work to be done
void Individual::mutate() {
    int face = rand() % 6;
    int transform = rand() % mutations.size();
    std::vector<std::pair<int, int>> seq;
    std::vector<plane> planes = planesinfront;
    int k = rand() % 4;
    for (int i = 0; i < k; i++) {
        int temp = planes[face].up;
        planes[face].up = planes[face].left;
        planes[face].left = planes[face].down;
        planes[face].down = planes[face].right;
        planes[face].right = temp;
    }
    for (char c : mutations[transform]) {
        switch (c) {
            case 'F':
                seq.push_back({face, 1});
                break;
            case 'R':
                seq.push_back({planes[face].right, 1});
                break;
            case 'L':
                seq.push_back({planes[face].left, 1});
                break;
            case 'B':
                seq.push_back({planes[face].back, 1});
                break;
            case 'U':
                seq.push_back({planes[face].up, 1});
                break;
            case 'D':
                seq.push_back({planes[face].down, 1});
                break;
            case 'i':
                seq.back().second = 3;
                break;
            case '2':
                seq.back().second = 2;
                break;
            default:
                break;
        }
    }
    for (auto gene : seq) {
        genom_lenght++;
        genes.push_back(gene);
    }
}

std::vector<std::pair<int, int>>& Individual::getGenes() {
    return genes;
}

long long Individual::getFitness() {
    return fitness_score;
}

Population::Population(int n, const Cube& cube_) : population_count(n), cube(cube_) {
    fittest = Individual();
    fittest.empty();
    fittest.fitness(cube);
    population = std::vector<Individual>(n);
    init();
}


void Population::evoluteNewPopulation() {
    ++generations;
    evoluate();
    
    sortByFittest();
   
    selection();
   
    population = newPopulation;
    sortByFittest();
    newPopulation.clear();
    
    if (generations > 80) {
        init();
    }
}

void Population::evoluate() {
    average_fitness = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 1; j < 4; j++) {
            Individual ind = fittest;
            ind.genom_lenght++;
            ind.genes.push_back({i, j});
            if (ind.fitness(cube) < fittest.fitness_score) {
                fittest = ind;
                break;
            }
        }
    }
    for (auto& individual : population) {
        if (individual.fitness_score == -1) {
            individual.fitness(cube);
        }
        average_fitness += individual.fitness_score / (double) population_count;
        if (individual.fitness_score < fittest.fitness_score || (fittest.fitness_score == individual.fitness_score && fittest.genom_lenght > individual.genom_lenght)) {
            fittest = individual;
        }
    }
    for (auto& individual : population) {
        individual.probability = individual.fitness_score / (average_fitness * population_count);
    }
}
void Population::selection() {
    for (int i = 0; i < population_count / 10 ; i++) {
        newPopulation.push_back(population[i]);
    }
    for (int i = 0; i < population_count / 10; i++) {
        for (int j = 0; j < 9; j++) {
            Individual ind = population[i];
            ind.mutate();
            if (ind.fitness(cube) < population[i].fitness_score) {
               population[i] = ind;
            }
            newPopulation.push_back(ind);
        }
    }
    
}

void Population::init() {
    fittest.empty();
    generations = 0;
    for (int i = 0; i < population_count; i++) {
        population[i] = Individual();
    }
    fittest.fitness(cube);
}

long Population::getBestScore() {
    return fittest.fitness_score;
}

int Population::getGenerations() {
    return generations;
}

bool Population::found() {
    return fittest.fitness_score == 0;
}

long compare(Individual& a, Individual& b) {
    return a.fitness_score < b.fitness_score;
}

void Population::sortByFittest() {
    std::sort(population.begin(), population.end(), compare);
    population.resize(population_count);
}

Individual& Population::getFittest() {
    return fittest;
}
