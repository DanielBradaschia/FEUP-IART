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


vector<Photo> photoList;
vector<Slide> bestGen, model;
vector<vector<Slide>> currGen, nextGen;
vector<int> organismsScore;
int totalOfFitnesses, numOrganisms;
int cz;

//Functions
vector<string> generateTokens(string line);
Photo populatePhotos(vector<string> photo, int id);
int DoOneRun();
void ProduceNextGeneration();
vector<Slide> generateSlideshow(vector<Photo> photoList);
int calculateScore(vector<Slide> slideshow);
void printSlide(Slide res);
bool EvaluateOrganisms();
int SelectOneOrganism();
vector<Slide> hillClimbing();
Slide mutateSlide();

int myrandom(int i) { return rand() % i; }


int main()
{
    string fileRead, line, numPhotos;
    ifstream infile;
    ofstream outputFile;

    int answer;

    totalOfFitnesses = 0;
    srand(time(0));
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

    cz = 0;
    int id = 1;

    while(i > 0){
        getline(infile, line);
        
        vector<string> aux = generateTokens(line);
        
        // initialize the normal organisms
        photoList.push_back(populatePhotos(aux, id));

        i--;
        id++;
    }

    model = hillClimbing();

    //Fill organisms
    for(int j = 0; j < 5; j++)
    {
        currGen.push_back(generateSlideshow(photoList));
        nextGen.push_back(generateSlideshow(photoList));
        organismsScore.push_back(calculateScore(currGen.at(j)));
        random_shuffle(photoList.begin(), photoList.end(), myrandom);
    }

    answer = DoOneRun();

    cout << "Best Generation was: " << answer << endl;
    //write slideshow
    outputFile.open("slideshow.txt");
    outputFile << bestGen.size() << endl;
    for (int i = 0; i < bestGen.size(); i++)
    {
        outputFile << bestGen.at(i).photo1Id;
        if (bestGen.at(i).photo2Id == -1)
            outputFile << endl;
        else
            outputFile << " " << bestGen.at(i).photo2Id << endl;
    }

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

void printSlide(Slide res){
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


void ProduceNextGeneration(){
    int organism;
    int gene;
    int parentOne;
    int parentTwo;
    int crossoverPoint;
    int mutateThisGene;

    //fill the nextGeneration data structure with the
    //children
    for (organism = 0; organism < currGen.size(); ++organism)
    {
        parentOne = SelectOneOrganism();
        parentTwo = SelectOneOrganism();
        crossoverPoint = rand() % currGen.size();

        for(gene = 0; gene < currGen.at(0).size(); ++gene){
            // copy over a single gene
            mutateThisGene = rand() % (int)(1.0 / MUTATION_RATE);
            if (mutateThisGene == 0)
            {

                // we decided to make this gene a mutation
                nextGen[organism][gene] = mutateSlide();
            }
            else
            {
                // we decided to copy this gene from a parent
                if (gene < crossoverPoint)
                {
                    nextGen.at(organism).at(gene) = currGen.at(parentOne).at(gene);
                }
                else
                {
                    nextGen.at(organism).at(gene) = currGen.at(parentTwo).at(gene);
                }
            }
        }
    }
    // copy the children in nextGeneration into
    // currentGeneration
    for(organism = 0; organism < 5; ++organism){
        for (gene = 0; gene < currGen.at(0).size(); ++gene)
        {
            currGen[organism][gene] = nextGen[organism][gene];
        }
    }
}

int SelectOneOrganism(){
    int runningTotal;
    int randomSelectPoint;
    int o = 0;

    runningTotal = 0;
    randomSelectPoint = rand() % (totalOfFitnesses + 1);

    for (; o < currGen.size()-1; ++o)
    {
        runningTotal += organismsScore.at(0);
        if(runningTotal >= randomSelectPoint)
         return o;
    }
    return o;
}


bool EvaluateOrganisms(){
    int organism;
    int gene;
    int currentOrganismsFitnessTally;
    int ret = false;

    for(int i = 0; i < currGen.size(); i++)
    {
        currentOrganismsFitnessTally = calculateScore(currGen.at(i));
        totalOfFitnesses += currentOrganismsFitnessTally;

        if (currentOrganismsFitnessTally > calculateScore(model))
        {
            bestGen = currGen.at(i);
            ret = true;
        }    
    }

    cz++;

    if(ret == true){
        cout << "Final Score: " << calculateScore(bestGen) << endl;
    }

    return ret;
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

int calculateScore(vector<Slide> slideshow){
    int res = 0;

    for(int i = 0; i < slideshow.size()-1; i++)
    {
        vector<string> vector1 = slideshow.at(i).tags, vector2 = slideshow.at(i + 1).tags;
        vector<string> difference1, difference2, intersection;

        // Sort the vectors
        sort(vector1.begin(), vector1.end());
        sort(vector2.begin(), vector2.end());

        //tags in vector1 and not in vector2
        set_difference(vector1.begin(), vector1.end(), vector2.begin(), vector2.end(), back_inserter(difference1));

        //tags in vector2 and not in vector1
        set_difference(vector2.begin(), vector2.end(), vector1.begin(), vector1.end(), back_inserter(difference2));

        //tags in common
        set_intersection(vector1.begin(), vector1.end(), vector2.begin(), vector2.end(), back_inserter(intersection));

        size_t aux = min(difference1.size(),difference2.size());
        
        res += min(aux, intersection.size());
    }
    return res;
}

vector<Slide> hillClimbing(){

    vector<Slide> current = generateSlideshow(photoList);
    vector<Slide> neighbor;
    
    int currentScore, neighborScore;

    while (true)
    {
        random_shuffle(photoList.begin(), photoList.end(), myrandom);
        neighbor = generateSlideshow(photoList);
        currentScore = calculateScore(current);
        neighborScore = calculateScore(neighbor);

        if (neighborScore <= currentScore)
        {
            cout << "Initial Score: " << currentScore << endl;
            return current;
        }

        current = neighbor;
    }

    return current;
}

Slide mutateSlide(){

    random_shuffle(photoList.begin(), photoList.end());

    vector<Slide> aux = generateSlideshow(photoList);

    return aux.at(rand() % aux.size());
}