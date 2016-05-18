#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include"Trie.h"

using namespace std;

inline const char * const BoolToString(bool b)
{
    return b ? "true" : "false";
}

void edits(std::string word, std::vector<string> &result)
{
    for (string::size_type i = 0;i < word.length(); i++) {
        result.push_back(word.substr(0, i) + word.substr(i + 1)); //deletions
        cout<<word.substr(0, i) + word.substr(i + 1)<<endl;
    }
    for (string::size_type i = 0;i < word.size() - 1;i++){
        result.push_back(word.substr(0, i) + word[i+1] + word.substr(i + 2)); //transposition
        cout<<word.substr(0, i) + word[i+1] + word.substr(i + 2)<<endl;
    }

    for (char j = 'a';j <= 'z';++j) {
        for (string::size_type i = 0; i < word.size(); i++) {
            result.push_back(word.substr(0, i) + j + word.substr(i + 1)); //alterations
        }
        for (string::size_type i = 0; i < word.size() + 1; i++) {
            result.push_back(word.substr(0, i) + j + word.substr(i)); //insertion
        }
    }
}


bool loadDictionary(Trie* trie,string filename)
{

    ifstream words;

    words.open(filename.c_str());

    if(!words.is_open())
    {
        cout<<"Dictionary file Not Open"<<endl;
        return false;
    }

    while(!words.eof())
    {
        char s[100];
        words >> s;
        trie->addWord(s);
    }
    return true;
}

bool loadAnotherDictionary(Trie* trie,string filename)
{

    ifstream words;

    words.open(filename.c_str());

    if(!words.is_open())
    {
        cout<<"Dictionary file Not Open"<<endl;
        return false;
    }

    while(!words.eof())
    {
        string s,reqS="";
        words >> s;
        for(int i=0;s.at(i)!=',';i++){
            reqS = reqS+s.at(i);
        }
        reqS = reqS.substr(5,reqS.length()-4);
        trie->addWord(reqS);
    }
    return true;
}



int main()
{
    Trie* trie = new Trie();
    int mode;

    cout<<"Loading dictionary"<<endl;

//    bool isDictionaryLoaded = loadDictionary(trie,"/Users/amitshekhar/Documents/Development/C++/WordSuggestions/wordlist.txt");
    bool isDictionaryLoaded = loadAnotherDictionary(trie,"/Users/amitshekhar/Documents/Development/C++/WordSuggestions/en_wordlist.txt");

    cout<<"isDictionaryLoaded : "<<BoolToString(isDictionaryLoaded)<<endl;

    while(1)
    {
        cout<<endl<<endl;
        cout<<"Interactive mode,press "<<endl;
        cout<<"1: Check for word correctness and and suggest corrections"<<endl;
        cout<<"2: Auto Complete Feature"<<endl;
        cout<<"3: Load input file to check correctness and suggest corrections"<<endl;
        cout<<"4: Load input file for auto complete"<<endl;
        cout<<"5: Quit"<<endl;
        cout<<"Input file : input.txt"<<endl;
        cout<<"Output file will be : output.txt"<<endl;

        cin>>mode;

        switch(mode)
        {
            case 1://word correctness
            {
                string s;
                cin>>s;
                transform(s.begin(), s.end(), s.begin(), ::tolower);
                int count=0;
                set<string> suggestions;
                vector<string> edit_list;
                if(trie->searchWord(s)){
                    //	output<<s<<" Correct word "<<endl;
                    cout<<s<<"  found in Dictionary"<<endl;
                }
                else
                {
                    edits(s,edit_list);
                    cout<<"edit list length : "<<edit_list.size()<<endl;
                    for(unsigned int i=0;i<edit_list.size();i++)
                    {
                        cout<<edit_list[i]<<endl;
                        if(trie->searchWord(edit_list[i]))
                        {
                            suggestions.insert(edit_list[i]);
                            count++;
                            if(count>10)
                                break;
                        }
                    }
                    if(count==0){
                        cout<<"No suggestions"<<endl;
                    }
                    else
                    {
                        cout<<"Did you mean"<<endl;
                        set<string>::iterator itr;
                        for(itr=suggestions.begin();itr!=suggestions.end();itr++)
                            cout<<*itr<<endl;
                    }

                }

            }
                continue;

            case 2://Auto complete
            {
                string s;
                cin>>s;
                transform(s.begin(), s.end(), s.begin(), ::tolower);

                vector<string> autoCompleteList;
                trie->autoComplete(s,autoCompleteList);

                if(autoCompleteList.size()==0)
                {
                    cout<<"No suggestions"<<endl;
                }
                else
                {
                    cout<<"Autocomplete reply :"<<endl;
                    for(int i=0;i<autoCompleteList.size();i++)
                    {
                        cout<<autoCompleteList[i]<<endl;
                    }
                }
            }
                continue;

            case 3:
            {
                ifstream input;
                ofstream output;

                input.open("input.txt");
                output.open("output.txt");

                if(!input.is_open())
                {
                    cout<<"Input file not found"<<endl;
                    continue;
                }

                if(!output.is_open())
                {
                    cout<<"Failed to open Output.txt"<<endl;
                    continue;
                }

                while(!input.eof())
                {
                    char d[300];
                    input >> d;
                    string s=d;
                    transform(s.begin(), s.end(), s.begin(), ::tolower);

                    int count=0;
                    set<string> suggestions;
                    vector<string> edit_list;
                    if(trie->searchWord(s)){
                        //	output<<s<<" Correct word "<<endl;
                        output<<s<<" found in Dictionary"<<endl;
                    }
                    else
                    {
                        edits(s,edit_list);

                        for(unsigned int i=0;i<edit_list.size();i++)
                        {
                            if(trie->searchWord(edit_list[i]))
                            {
                                suggestions.insert(edit_list[i]);
                                count++;
                                if(count>10)
                                    break;
                            }
                        }
                        if(count==0){
                            output<<s<<" No suggestions"<<endl;
                        }
                        else
                        {
                            output<<s<<" Did you mean"<<endl;
                            set<string>::iterator itr;
                            for(itr=suggestions.begin();itr!=suggestions.end();itr++)
                                output<<*itr<<endl;
                        }

                    }

                }//End of input file read

            }
                cout<<"Please check output.txt "<<endl;
                continue;
            case 4:
            {
                ifstream input;
                ofstream output;

                input.open("input.txt");
                output.open("output.txt");

                if(!input.is_open())
                {
                    cout<<"Input file not found"<<endl;
                    continue;
                }

                if(!output.is_open())
                {
                    cout<<"Failed to open Output.txt"<<endl;
                    continue;
                }

                while(!input.eof())
                {
                    char d[300];
                    input >> d;
                    string s=d;
                    transform(s.begin(), s.end(), s.begin(), ::tolower);
                    vector<string> autoCompleteList;
                    trie->autoComplete(s,autoCompleteList);

                    if(autoCompleteList.size()==0)
                    {
                        output<<s<<"No suggestions"<<endl;
                    }
                    else
                    {
                        output<<s<<" Autocomplete reply :"<<endl;
                        for(int i=0;i<autoCompleteList.size();i++)
                        {
                            output<<autoCompleteList[i]<<endl;
                        }
                    }
                }
            }
                cout<<"Please check output.txt "<<endl;
                continue;
            case 5: delete trie;
                return 0;
            default:
                continue;
        }


    }
}