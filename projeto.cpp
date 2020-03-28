#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include <cctype>
#include <stdbool.h>

#define ALLELES 3
#define MUTATION_RATE 0.001

using namespace std;

//Global Variables
typedef struct
{
    string type;
    int numTags;
    vector<string> tags;
} Photo;

typedef struct
{
    int photo1Id, photo2Id, score, isHorizontal;
} Slide;

vector<Photo> currGen, nextGen, model;
vector<int> organismsFitnesses;
int totalOfFitnesses, numOrganisms;

//Functions
vector<string> generateTokens(string line);
Photo populatePhotos(vector<string> photo);
int DoOneRun();
int EvaluateOrganisms();
void ProduceNextGeneration();

int main()
{
    string fileRead, line, numPhotos;
    ifstream infile;

    int answer;

    //Leitura do arquivo
    cout    << "Input file name: " << endl;
    getline(cin, fileRead);
    infile.open(fileRead);
    if (infile.fail())
    {
        cerr << "Error opening file: " << fileRead << endl;
        exit(1);
    }

    getline(infile, numPhotos);
    int i = stoi(numPhotos);
    numOrganisms = i;

    while(i > 0){
        getline(infile, line);
        
        vector<string> aux = generateTokens(line);
        
        // initialize the normal organisms
        currGen.push_back(populatePhotos(aux));

        i--;
    }

    answer = DoOneRun();
    return 0;
}

void printPhoto(Photo res){
    cout << "PHOTO {" << endl;
    cout << "TYPE : " << res.type << endl;
    cout << "TAGS : ";
    for (size_t i = 0; i < res.tags.size(); i++){
        cout << res.tags[i] << " ";
    }
    cout << endl;
    cout << "}" << endl;
}

vector<string> generateTokens(string line){
    stringstream check1(line);
    string intermediate;
    vector<string> tokens;

    // Tokenizing
    while (getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    return tokens;
}

Photo populatePhotos(vector<string> photo){

    Photo res;

    res.type = photo[0];
    res.numTags = stoi(photo[1]);

    for (int i = 0; i < res.numTags; i++)
    {
        res.tags.push_back(photo[i+2]);
    }

    return res;
}

int DoOneRun(){
    int gen = 1;
    bool ans = false;

    while (true)
    {
        ans = EvaluateOrganisms();
        if (ans == true)
            return gen;
        ProduceNextGeneration();
        ++gen;
    }
}

int EvaluateOrganisms(){
    int organism;
    int gene;
    int currentOrganismsFitnessTally;

    totalOfFitnesses = 0;

    for (organism = 0; organism < numOrganisms; ++organism)
    {
        currentOrganismsFitnessTally = 0;

        // tally up the current organism's fitness
        for (gene = 0; gene < model.size(); ++gene)
        {
            /*Fazer operador de comparacao de structs?*/
            if(currGen.at(gene).numTags == model.at(gene).numTags && currGen.at(gene).tags == model.at(gene).tags && currGen.at(gene).type == model.at(gene).type)
            {
                ++currentOrganismsFitnessTally;
            }        
        }

        // save the tally in the fitnesses data structure
        // and add its fitness to the generation's total
        organismsFitnesses[organism] = currentOrganismsFitnessTally;
        totalOfFitnesses += currentOrganismsFitnessTally;

        // check if we have a perfect generation
        if (currentOrganismsFitnessTally == model.size())
        {
            return true;
        }
    }
    return false;
}

void ProduceNextGeneration(){
    int organism;
    int gene;
    int parentOne;
    int parentTwo;
    int crossoverPoint;
    int mutateThisGene;

    //fill the nextGeneration data structure with the
    //children
    for (organism = 0; organism < numOrganisms; ++organism)
    {
        parentOne = SelectOneOrganism();
        parentTwo = SelectOneOrganism();
        crossoverPoint = rand() % model.size();

        for(gene = 0; gene < model.size(); ++gene){
            // copy over a single gene
            mutateThisGene = rand() % (int)(1.0 / MUTATION_RATE);
            if(mutateThisGene == 0){
                // we decided to make this gene a mutation
                nextGen.at(gene).numTags = rand() % ALLELES;
            } else {
                // we decided to copy this gene from a parent
                if(gene < crossoverPoint){
                    //nextGen[organism][gene] = currGen[ParentOne][gene];
                } else {
                    //nextGen[organism][gene] = currGen[ParentTwo][gene];
                }
            }
        }
    }
    // copy the children in nextGeneration into
    // currentGeneration
    for(organism = 0; organism < numOrganisms; ++organism){
        for(gene = 0; gene < model.size(); ++gene){
            //currGen[organism][gene] = nextGen[organism][gene];
        }
    }
}

int SelectOneOrganism(){
    int organism;
    int runningTotal;
    int randomSelectPoint;

    runningTotal = 0;
    randomSelectPoint = rand() % (totalOfFitnesses + 1);

    for (organism = 0; organism < numOrganisms; ++organism){
        runningTotal += organismsFitnesses[organism];
        if(runningTotal >= randomSelectPoint) return organism;
    }
    
}
