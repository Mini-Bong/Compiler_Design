#include<bits/stdc++.h>
using namespace std;
int i=0,j=0, number_of_terminals = 0;
char pb[10][10];// It will store the production rules
char non_ter[10]; // It will store the non termination symbol
int number_of_production;
bool non_terminal(char);
void FIRST(vector<char>, char);
void FOLLOW(char);
int column_index(char);
int row_index(char);
vector<char> first_set; // for storing the first of terminal or non terminal
vector<char> follow_set; // for storing the follow of non_terminal
map<pair<int, int>, string> parsing_table; // for storing the predictive parser table
vector<char> terminals; // for strong the terminals symbols
vector<char> nt;// it storing the only one occurrence of non_terminal symbol
void pre_parser_table_generator();
void non_recursive_predictive_parser(string);
int main()
{
    //NOTE: Epsilon production rule will be denoted by A->#
        cout<<"Enter the number of terminal symbols ";
        cin>>number_of_terminals;
        number_of_terminals++; // because for parsing table $ symbol will be extra terminating symbol
        terminals.resize(number_of_terminals);
        cout<<"Enter all terminals separated by space: ";
        for(int i=0; i<number_of_terminals-1; i++)
        {
            cin>>terminals[i];
        }
        terminals[number_of_terminals-1] = '$';
        printf("Enter the number of production rule:");
        scanf("%d",&number_of_production);
        printf("\nEnter the production and production body: ");
        for(int i=0; i<number_of_production; i++)
        {
                printf("\nProduction:");
                cin>>non_ter[i];
                printf("\nProduction body:");
                printf("%c ->",non_ter[i]);
                cin>>pb[i];
                printf("\n");
        }
        char c;
        char check = 'y';
        while(check!='n')
        {
            cout<<"Enter Terminal or nonterminal to find first ";
            cin>>c;
            FIRST(first_set,c);
            cout<<"{";
            for(int i = 0; i<first_set.size(); i++)
            {
                cout<<first_set[i]<<",";
            }
            cout<<"}\n";
            first_set.clear();

            // please do not pass terminals as input in FOLLOW() function
            /*if(non_terminal(c))
            {
                FOLLOW(c);
                cout<<"{";
                for(int i = 0; i<follow_set.size(); i++)
                {
                    cout<<follow_set[i]<<",";
                }
                cout<<"}\n";
            }
            follow_set.clear();
            cout<<"\nDo you want to continue?? press.... y/n....";
            cin>>check;
        }
        /*pre_parser_table_generator();
        string str;
        cout<<"Enter the string which will going to check: ";
        cin>>str;
        str = str + "$";
        non_recursive_predictive_parser(str);*/
        }
        return 0;
}
void FIRST(vector<char> v, char ch)
{
    vector<char> sub_first;
    if(!non_terminal(ch))
    {
        first_set.push_back(ch);
        return;
    }
    if(non_terminal(ch))
    {
        int pb_body_pointer = 0;
        for(int i = 0; i<number_of_production; i++)
        {
            if(ch == non_ter[i])
            {
                if(pb[i][0]=='#')
                {
                    first_set.push_back('#');
                }
                else
                {
                    int j = 0;
                    while(pb[i][j]!='\0')
                    {
                        int epsilon = 0;
                        FIRST(sub_first, pb[i][j]);
                        for(int a =0; a<sub_first.size(); a++)
                        {
                            first_set.push_back(sub_first[a]);
                            if(sub_first[a]=='#')
                            {
                                epsilon = 1;
                            }
                        }
                        if(!epsilon)
                        {
                            break;
                        }
                        j++;
                    }
                }
            }
        }
    }
}/*

void FOLLOW(char ch)
{
    int flag = 0;
    if(non_ter[0]==ch)// It will check whether it is a starting symbol or not
    {
        if(!(count(follow_set.begin(), follow_set.end(), '$')))
        {
            follow_set.push_back('$');
        }
    }
  for(int i=0; i<number_of_production; i++)
  {
    for(int j=0; j<strlen(pb[i]); j++)
      {
        if(pb[i][j]==ch)
        {
            if(pb[i][j+1]!='\0')
            {
                first_set.clear();
                FIRST(first_set, pb[i][j+1]);
                for(int a =0; a<first_set.size(); a++)
                {
                    if(first_set[a]!='#')
                    {
                        if(!(count(follow_set.begin(), follow_set.end(), first_set[a])))
                        {
                            follow_set.push_back(first_set[a]);
                        }
                    }
                    if(first_set[a]=='#')
                    {
                        flag = 1;
                    }
                }
                first_set.clear();
            }

            if(non_ter[i]!=ch && (pb[i][j+1]=='\0' || flag ==1))
            {
                FOLLOW(non_ter[i]);
            }
        }
     }
  }
}

void pre_parser_table_generator()
{
    for(int i = 0; i<strlen(non_ter); i++)
    {
        if(!(count(nt.begin(), nt.end(), non_ter[i])))
        {
            nt.push_back(non_ter[i]);
        }
    }
    for(int i =0; i<=nt.size(); i++)
     {
         for(int j =0; j<=number_of_terminals; j++)
         {
             parsing_table[{i,j}] = "ERR";
         }
     }
     parsing_table[{0,0}] = "Ptbl";
    for(int i =0; i<number_of_terminals; i++)
    {
        string str(1, terminals[i]); // for converting char to string
        parsing_table[{0,i+1}] = str;
    }
    for(int i =0; i<nt.size(); i++)
    {
        string str(1, nt[i]); // for converting char to string
        parsing_table[{i+1,0}] = str;
    }
    int pb_pointer = 0;
    for(int i=0; i<strlen(non_ter); i++) // for each production rule A->a in grammar G
    {
        int index1 = row_index(non_ter[i]);
        int flag = 0;
        FIRST(first_set, pb[i][pb_pointer]);
        for(int j = 0; j<first_set.size(); j++)
        {
            if(first_set[j]!='#')
            {
                int index2 = column_index(first_set[j]);
                parsing_table[{index1+1, index2+1}] = pb[i];
            }
            if(first_set[j]=='#')
            {
                flag = 1;
            }
        }
        first_set.clear();
        if(flag == 1)
        {
            FOLLOW(non_ter[i]);
            for(int j = 0; j<follow_set.size(); j++)
            {
                int index2 = column_index(follow_set[j]);
                parsing_table[{index1+1, index2+1}] = pb[i];
                if(follow_set[j]=='$')
                {
                    int index3 = column_index(follow_set[j]);
                    parsing_table[{index1+1, index3+1}] = pb[i];

                }
            }
            follow_set.clear();
        }
    }
    cout<<"(ERR) is Denoting the error........\n\n";
     for(int i =0; i<=nt.size(); i++)
     {
         for(int j =0; j<=number_of_terminals; j++)
         {
             cout<<setw(7)<<parsing_table[{i,j}];
         }
         cout<<"\n\n";
     }
     cout<<"\n\n";
}

void non_recursive_predictive_parser(string str)
{
    stack<char> s; // stack used in predictive parser
    s.push('$');
    s.push(nt[0]);
    char X = s.top();   // top symbol of stack
    int string_pointer = 0;
    char ch;
    while(X!='$')
    {
        ch = str[string_pointer];
        if(X == ch)
        {
            s.pop();
            string_pointer++;
            cout<<ch<<" Matched\n";
        }
        else if(count(terminals.begin(), terminals.end(), X))
        {
            cout<<"ERROR\n";
            break;
        }
        else if(parsing_table[{row_index(X)+1, column_index(ch)+1}] == "ERR")
        {
            cout<<"THIS IS NOT A VALID STRING\n";
            break;
        }
        else
        {
            string res = parsing_table[{row_index(X)+1, column_index(ch)+1}];
            string res1 = parsing_table[{row_index(X)+1, 0}];
            cout<<res1<<"->"<<res<<"\n";
            s.pop();
            for(int i = res.length()-1; i>=0; i--)
            {
                if(res[i]!='#')
                {
                    s.push(res[i]);
                }
            }
        }
        X = s.top();
    }
}
*/
int row_index(char ch) // for finding the row_number in the parsing table
{
    for(int i = 0; i<nt.size(); i++)
    {
        if(nt[i]==ch)
        {
            return i;
            break;
        }
    }
}
int column_index(char ch) // for finding the column number in the parsing table
{
    for(int i = 0; i<terminals.size(); i++)
    {
        if(terminals[i]==ch)
        {
            return i;
            break;
        }
    }
}

bool non_terminal(char ch)
{
    for(int i = 0; i<strlen(non_ter); i++)
    {
        if(ch==non_ter[i])
        {
            return true;
        }
    }
    return false;
}
