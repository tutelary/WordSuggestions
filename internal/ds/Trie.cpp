//
// Created by Amit Shekhar on 15/05/16.
//

#include "Trie.h"
#include <iostream>
Trie::Trie()
{
    root = new Node();
}

Trie::~Trie()
{
    cout<<"Free memory of Trie"<<endl;
}

void Trie::addWord(string s)
{
    Node* current = root;

    if ( s.length() == 0 )
    {
        current->setWordMarker(); // an empty word
        return;
    }

    for ( int i = 0; i < s.length(); i++ )
    {
        Node* child = current->findChild(s[i]);
        if ( child != NULL )
        {
            current = child;
        }
        else
        {
            Node* tmp = new Node();
            tmp->setContent(s[i]);
            current->appendChild(tmp);
            current = tmp;
        }
        if ( i == s.length() - 1 )
            current->setWordMarker();
    }
}


bool Trie::searchWord(string s)
{
    Node* current = root;

    while ( current != NULL )
    {
        for ( int i = 0; i < s.length(); i++ )
        {
            Node* tmp = current->findChild(s[i]);
            if ( tmp == NULL )
                return false;
            current = tmp;
        }

        return current->wordMarker();
    }

    return false;
}

void Trie::parseTree(Node *current, char *s,std::vector<string> &res,bool& loop)
{
    char k[100]={0};
    char a[2]={0};
    if(loop)
    {
        if(current!=NULL)
        {
            if(current->wordMarker()){
                res.push_back(s);
                if(res.size()>15)
                    loop=false;
            }
            vector<Node *> child=current->children();
            for(int i=0;i<child.size() && loop;i++){
                strcpy(k,s);
                a[0]=child[i]->content();
                a[1]='\0';
                strcat(k,a);
                if(loop)
                    parseTree(child[i],k,res,loop);
            }

        }
    }
}