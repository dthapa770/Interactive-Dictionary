// Dictionary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <set>

using namespace std;
int keywordsNumbers;
string trimString(string);
string partOfSpeeches[] = { "adjective", "adverb", "conjunction", "interjection", "noun", "preposition", "pronoun",
                           "verb" };

//class to make object for our txt file
class DictionaryItem {
private:
    string keyword{""};
    string definition{ "" };
    string partOfSpeech{ "" };
public:

 
    DictionaryItem() : keyword(""), partOfSpeech(""), definition("") {};

    DictionaryItem(string keyword,  string partOfSpeech, string definition) :
        keyword(keyword),  partOfSpeech(partOfSpeech), definition(definition) {};

   string getKeyword() const {
        return this->keyword;
    }
   string getPartOfSpeech() const {
      return this->partOfSpeech;
    }
   string getDefinition() const {
        return this->definition;
    }

    void print() const {
        cout<< keyword << " [" << partOfSpeech << "] : " << definition<<endl;
    }
};

string toLowerCase(string str) {
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }

    return str;
}

//check if the input is part of speech
bool isPartOfSpeech(string userInputForSpeech) {
    for (int i = 0; i < 8; i++) {
        if (userInputForSpeech == partOfSpeeches[i]) {
            return true;
        }
    }
    return false;
};

//trim string of txt file
string trimString(string arg) {
    if (arg.length() >= 2) {
        if (arg[0] == ' ') {

            arg = arg.substr(1);
        }
        if (arg[arg.length() - 1] == ' ') {

            arg = arg.substr(0, arg.length() - 1);
        }
    }
    return arg;
}

//main container that holds  keys and values of DictionaryItem
multimap<string, DictionaryItem> dictionaryContainer;

//load dictionary into DictionaryITem
multimap<string, DictionaryItem> loadDictionary(string fileLocation) {
 
    cout << "! Opening data file.... " << fileLocation << endl;
    ifstream ioFile;
    ioFile.open(fileLocation, ios::in);
    while (ioFile.fail()) {
        cout << "<!>ERROR<!>  ===> File could not be opened." << endl;
        cout << "<!>ERROR<!>  ===> Provided file path: " << fileLocation << endl;
        cout << "<!>Enter the CORRECT data file path: ";
        ioFile.close();
        getline(cin, fileLocation);
        ioFile.open(fileLocation, ios::in);

        //exit(EXIT_FAILURE);
    }
    cout << "! Loading data..." << endl;

    keywordsNumbers = 0;
    string currentLine{ "" };
    string finalResult;
    while (getline(ioFile, currentLine)) {
        keywordsNumbers++;
        int lastPos = currentLine.find('|');
        string keywords{ trimString(currentLine.substr(0, lastPos)) };
     
        currentLine.erase(0, lastPos + 1);

        stringstream stream(currentLine);
        string partOfSpeechAndDefinitions{ "" };

        while (getline(stream, partOfSpeechAndDefinitions, '|')) {
            string partOfSpeeches{ trimString(partOfSpeechAndDefinitions.substr(0, partOfSpeechAndDefinitions.find("-=>>"))) };
            string definitions{ trimString(partOfSpeechAndDefinitions.substr((partOfSpeechAndDefinitions.find("-=>>") + 4))) };
        
            dictionaryContainer.insert(make_pair(keywords, *(new DictionaryItem( keywords ,partOfSpeeches, definitions))));

        }
    }
    ioFile.close();
    cout << "! Closing Data File ......" << fileLocation << endl;;
    return dictionaryContainer;

    }

//split input frm user into vector indexes
vector<string>splitInputFromUser(string input, const char& ch) {
    stringstream ss(input);
    string item;
    vector<string> tokens;
    while (getline(ss, item, ch)) {
        tokens.push_back(item);
    }
    return tokens;
}
  
//print results
    void printResults(multimap<string, DictionaryItem> results) {
        if (results.empty()) {
            cout << " |\n<Not found> To be considered for next release. Thank You\n |\n |\n";
            cout << "PARAMETER HOW-TO, please enter: \n" << endl;
            cout << "1. A search key -then 2. An optional part of speech -then\n" << endl;
            cout << "3. An optional 'distinct' -then 4. An optional 'reverse'\n" << endl;
        }
        for (multimap<string, DictionaryItem>::iterator itr = results.begin(); itr != results.end(); itr++) {
            itr->second.print();
        }
    }
    //get in reverse order
    multimap<string, DictionaryItem> getReverse(multimap<string, DictionaryItem> reverse) {
        multimap<string, DictionaryItem> reverseCollector;
        for (auto itr = reverse.rbegin(); itr != reverse.rend(); itr++) {
            reverseCollector.insert(*itr);
        }
        return reverseCollector;
    }

    //get distinct 
   

    multimap<string, DictionaryItem> getDistinctI(multimap<string, DictionaryItem> distinct) {
                multimap<string, DictionaryItem> distinctItems;
                for (auto item = distinct.begin(); item != distinct.end(); ++item)
                {
                    bool isDistinct = true;
                    for (auto previousItem = distinctItems.begin(); previousItem != distinctItems.end(); ++previousItem)
                    {
                        if (previousItem->second.getDefinition() == item->second.getDefinition() && previousItem->second.getPartOfSpeech() == item->second.getPartOfSpeech())
                        {
                            isDistinct = false;
                            break;
                        }
                    }
                    if (isDistinct)
                        distinctItems.insert({ item->first, item->second });
                }
                return distinctItems;      
    }

    //get part of speech
       multimap<string, DictionaryItem> getPartOfSpeech(multimap<string, DictionaryItem> speech, string partOfInputSpeech) {
           multimap<string, DictionaryItem> speechContainer;
           for (auto itr = speech.begin(); itr != speech.end(); itr++) {

               if ((itr->second).getPartOfSpeech() == toLowerCase(partOfInputSpeech)) {
                   speechContainer.insert(*itr);
               }
           }

           return speechContainer;
       };

   
//search for the input given by user
void  search(vector<string> input) {
        if (dictionaryContainer.empty()) {
            cout << "it's empty";
        }
        int inputSize = input.size();
       string tempKeyword = input[0];

       //temp collector to hold keyword values
        multimap<string, DictionaryItem>tempcollector;
        auto its = dictionaryContainer.equal_range(toLowerCase(tempKeyword));
        for (auto itr = its.first; itr != its.second; itr++) {
            tempcollector.insert(*itr);
        };
     
        while (true) {
            switch (inputSize) {
            case 1:              
                printResults(tempcollector);
                break;
            case 2:
                if ( isPartOfSpeech(toLowerCase(input[1]))) {
                    printResults(getPartOfSpeech(tempcollector, input[1]));
                }
                else if (toLowerCase( input[1]) == "distinct") {
                    printResults(getDistinctI(tempcollector));
                }
                else if (toLowerCase( input[1]) == "reverse") {
                    printResults(getReverse(tempcollector));
                }
                else {
                    cout << "<The entered 2nd parameter '" + input[1] + "' is NOT a part of speech.>" << endl;
                    cout << "<The entered 2nd parameter '" + input[1] + "' is NOT 'distinct'.>" << endl;
                    cout << "<The entered 2nd parameter '" + input[1] + "' is NOT 'reverse'.>" << endl;
                    cout << "<The 2nd parameter should be a part of speech or 'distinct' or 'reverse'.>" << endl;
                    cout << "|\n|\n";
                    inputSize--;
                    continue;
                }
                break;
            case 3:
                if ( toLowerCase(input[2]) == "distinct") {
                    printResults(getDistinctI(getPartOfSpeech(tempcollector, input[1])));
                }
                else if (toLowerCase(input[1]) == "distinct" && toLowerCase(input[2]) == "reverse") {
                    printResults(getReverse(getDistinctI(tempcollector)));
                }
                else if ( toLowerCase(input[2]) == "reverse") {
                    printResults(getReverse(getPartOfSpeech(tempcollector, input[1])));
                }
                else {
                    cout << "<The entered 3rd parameter '" + input[2] + "' is NOT 'distinct'.>" << endl;
                    cout << "<The entered 3rd parameter '" + input[2] + "' is NOT 'reverse'.>" << endl;
                    cout << "<The 3rd parameter should be  'distinct' or 'reverse'.>" << endl;
                    inputSize--;
                    continue;
                }
                break;
            case 4:
                if ( toLowerCase(input[2]) == "distinct" && toLowerCase( input[3]) == "reverse") {
                    printResults(getReverse(getDistinctI(getPartOfSpeech(tempcollector, input[1]))));
                }
                else if (toLowerCase(input[3]) == "reverse") {
                    printResults(getReverse(getPartOfSpeech(tempcollector, input[1])));
                }
                else {
                    cout << "<The entered 3rd parameter '" + input[3] + "' is NOT 'reverse'.>" << endl;
                    cout << "<The 4th parameter should be  'reverse'.>"<< endl;
                    inputSize--;
                    continue;
                }
                break;
            default:
                cout << "PARAMETER HOW-TO, please enter: \n" << endl;
                cout << "1. A search key -then 2. An optional part of speech -then\n" << endl;
                cout << "3. An optional 'distinct' -then 4. An optional 'reverse'\n" << endl;
                break;
            }
            break;
        }

    };

    int main() {
        int count = 1;
        string inputFromUser;
        string fLocation = "Data.CS.SFSU.txt"; // File location
        loadDictionary(fLocation);
        cout << "\n========DICTIONARY 340 JAVA======== \n";
        cout << "----------No. of definition: " << dictionaryContainer.size() << endl;
        cout << "----------No. of keyword: " << keywordsNumbers<<endl;
        cout << " \nsearch [" <<count<<"] :";
        getline(cin, inputFromUser);
            while (toLowerCase(inputFromUser) != "!q") {
                    if (inputFromUser == "!help" || inputFromUser.empty()) {
                        cout << "PARAMETER HOW-TO, please enter: \n" << endl;
                        cout << "1. A search key -then 2. An optional part of speech -then\n" << endl;
                        cout << "3. An optional 'distinct' -then 4. An optional 'reverse'\n" << endl;
                    }
                    else {
                        vector<string>inputContainer = splitInputFromUser(inputFromUser, ' ');
                        search(inputContainer);
                    }        
                    cout << "   |\n   |\n";
                    count++;
                        cout << "Search [" << count << "] : ";
                        getline(cin, inputFromUser);   
            }
         cout << "\n ------Thank You----- "; 
        return 0;
    };


