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
#include <algorithm>

#define ALLELES 3
#define MUTATION_RATE 0.001

using namespace std;

//Global Variables
typedef struct
{
    int id;
    string type;
    int numTags;
    vector<string> tags;
    bool used;
} Photo;

typedef struct
{
    int photo1Id, 
        photo2Id, 
        score, 
        isHorizontal;
    vector<string> tags;
} Slide;


vector<Photo> photoList, model;
vector<Slide> currGen, nextGen;
vector<int> organismsFitnesses;
int totalOfFitnesses, numOrganisms;

//Functions
vector<string> generateTokens(string line);
Photo populatePhotos(vector<string> photo, int id);
int DoOneRun();
int EvaluateOrganisms();
void ProduceNextGeneration();
vector<string> mergeTags(Photo p1, Photo p2);
vector<Slide> generateSlideshow(vector<Photo> photoList);
void printSlide(Slide res);

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

    int id = 1;

    while(i > 0){
        getline(infile, line);
        
        vector<string> aux = generateTokens(line);
        
        // initialize the normal organisms
        photoList.push_back(populatePhotos(aux, id));

        i--;
        id++;
    }
    currGen = generateSlideshow(photoList);
    for(int j = 0; j < currGen.size(); j++)
        printSlide(currGen.at(j));
    //answer = DoOneRun();
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

void printSlide(Slide res)
{
    cout << "Slide {" << endl;
    if(res.isHorizontal){
        cout << "id1: " << res.photo1Id << endl;
        cout << "TYPE : H" << endl;
    } else {
        cout << "id1 e id2: " << res.photo1Id << " " << res.photo2Id << endl;
        cout << "TYPE : V" << endl;
    }
    cout << "TAGS : ";
    for (size_t i = 0; i < res.tags.size(); i++)
    {
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

Photo populatePhotos(vector<string> photo, int id){

    Photo res;

    res.id = id;
    res.type = photo[0];
    res.numTags = stoi(photo[1]);

    for (int i = 0; i < res.numTags; i++)
    {
        res.tags.push_back(photo[i+2]);
    }

    res.used = false;

    return res;
}
/*
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
*/
vector<string> mergeTags(Photo p1, Photo p2){

    vector<string> res;
    vector<string>::iterator it;

    sort(p1.tags.begin(), p1.tags.end());
    sort(p2.tags.begin(), p2.tags.end());

    it = set_union(p1.tags.begin(), p1.tags.end(), p2.tags.begin(), p2.tags.end(), res.begin());
    res.resize(it-res.begin());

    return res;
}

vector<Slide> generateSlideshow(vector<Photo> photoList){
    //reset used attribute
    for (int i = 0; i < photoList.size(); ++i)
    {
        photoList[i].used = false;
    }

    vector<Slide> slideshow;
    int processed = 0;
    Photo horz, vert1, vert2;
    vert1.id = -1;
    slideshow.reserve(photoList.size());

    for (int i = 0; i < photoList.size(); i++)
    {
        Slide aux;
        if (photoList[i].type == "H")
        {
            
            photoList[i].used = true;
            horz = photoList[i];
            processed++;
            aux.photo1Id = horz.id;
            aux.photo2Id = -1;
            aux.tags = horz.tags;
            aux.isHorizontal = 1;
            slideshow.push_back(aux);
        }

        if (photoList[i].type == "V")
        {
            if(vert1.id != -1)
            {
                vert2 = photoList[i];
                aux.photo1Id = vert1.id;
                aux.photo2Id = vert2.id;
                //aux.tags = vert1.tags;copy(source.begin(), source.end(), std::back_inserter(destination))
                aux.tags = vert1.tags;
                copy(vert2.tags.begin(), vert2.tags.end(), back_inserter(aux.tags));
                sort(aux.tags.begin(), aux.tags.end());
                aux.tags.erase(unique(aux.tags.begin(), aux.tags.end()), aux.tags.end());
                aux.isHorizontal = 0;
                vert1.id = -1;
                slideshow.push_back(aux);
            }
            else
            {
                photoList[i].used = true;
                vert1 = photoList[i];
                processed++;
            }
        }
    }

    return slideshow;

}
