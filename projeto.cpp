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

using namespace std;

//Global Variables
typedef struct
{
    string type;
    int numTags;
    vector<string> tags;
} Photo;

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

int EvaluateOrganisms()
{
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
            if(currGen.at(gene).numTags == model.at(gene).numTags && currGen.at(gene).tags == model.at(gene).tags && currGen.at(gene).type == model.at(gene).type )
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

void ProduceNextGeneration()
{

}
