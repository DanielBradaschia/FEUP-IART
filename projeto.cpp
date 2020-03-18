#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>
#include <chrono>
#include <cctype>

using namespace std;

//Global Variables
typedef struct
{
    string type;
    int numTags;
    vector<string> tags;
} Photo;

//Functions
vector<string> generateTokens(string line);
Photo populatePhotos(vector<string> photo);



int main()
{
    string fileRead, line, numPhotos;
    ifstream infile;
    vector<Photo> photos;

    //Leitura do arquivo
    cout << "Input file name: " << endl;
    getline(cin, fileRead);
    infile.open(fileRead);
    if (infile.fail())
    {
        cerr << "Error opening file: " << fileRead << endl;
        exit(1);
    }

    getline(infile, numPhotos);
    int i = stoi(numPhotos);

    while(i > 0){
        getline(infile, line);
        //cout << line << endl;
        vector<string> aux = generateTokens(line);
        
        /*
        for (int j = 0; j < aux.size(); j++)
            cout << "aux: "<< aux[j] << endl;
        */

        photos.push_back(populatePhotos(aux));       

        i--;
    }

    cout << photos.size() << endl;

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
   
    /*
    cout << "TESTE" << endl;
    cout << res.type << endl;
    cout << res.numTags << endl;
    for (int i = 0; i < res.numTags; i++)
    {
        cout << res.tags[i] << endl;
    }
    */

    return res;
}