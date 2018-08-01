/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

/*=========================================================================
  Constants and definitions:
==========================================================================*/

#define len 9
#define maxword 30
#define alphabet 'a'-'A'

/*-------------------------------------------------------------------------
  The main program. (describe what your program does here)
 -------------------------------------------------------------------------*/
/*
int strcmp(char *str1, char *str2)
{
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}
*/


 int Upper_Or_Lower (char let) //0 if caps 1 if lower. 2 if hypen. 3 other
    {
        if(let>='A' && let<='Z') //caps
            return 0;
        else
            {
                if (let>='a' && let<='z') //lower
                    return 1;
                else if (let=='-') //other
                    return 2; // hyphen
                else
                    return 3; //not letter or hyphen
            }
    }


int Find_Word_Length (char* ptr) //find length of a word given a pointer to the string
    {
        int i=0;
        while(*(ptr+i)) //until reaches ascii of 0 (terminator)
            i++;
        return i;
    }
/*
void Turn_to_Lower (char* ptr) //FUNCTION PROBABLY NOT NEEDED
    {
        int length;
        length=Find_Word_Length(ptr);
        for(int i=0; i<length; i++)
            {
                if(ptr[i]>='A' && ptr[i]<='Z')
                    s[i]=ptr[i]+alphabet;
                else
                    s[i]=ptr[i];
            }
    }
*/

int Is_Exact_Or_Prefix (char *word1, char *word2) //word1 is input, word2 is refrence
    {
        int ref_len, input_len;
        input_len=Find_Word_Length(word1);
        ref_len=Find_Word_Length(word2);
        if(input_len<ref_len) //avoid overflow
            return 0;
        else //if input word is longer or equal length
            {
                for(int i=0; i<ref_len; i++)
                    {
                        if(Upper_Or_Lower(word1[i])==0) //if uppercase
                            {
                                if((word1[i]+alphabet)!=word2[i])
                                    return 0;
                            }
                        else if(word1[i]!=word2[i]) //if not upper
                            return 0;
                    }
            }
        return 1;
    }


int backwords_match (char *word1, char *word2) //word1 is input, word2 is refrence. should get lowecase words
    {
        int ref_len, input_len;
        input_len=Find_Word_Length(word1);
        ref_len=Find_Word_Length(word2);
        if(input_len<ref_len) //avoid overflow
                return 0;
        else //if input word is longer or equal length
            {
                for(int i=0; i<ref_len; i++)
                    {
                        if(Upper_Or_Lower(word1[input_len-1-i])!=0) //not upper
                        {
                            if(word1[input_len-1-i]!=word2[ref_len-1-i])
                                return 0;
                        }
                        else
                        {
                            if(word1[input_len-1-i]+alphabet!=word2[ref_len-1-i])
                                return 0;
                        }
                    }
            }
        return 1;
    }

/*
int main() //for function tests
{
    int a;
    char s1[]="royWaDe";
    char s2[]="wade"; //only small
    a=backwords_match(s1,s2);
    printf("val is %d", a);
    return 0;
}
*/



int binarySearch(char* dic[], int n, char* w, int index) //recieves dictionary and its length, and the word to search, and the letter
{
   int low=0, mid, high=n-1, word_len, i, j;
   word_len=Find_Word_Length(w);
   if(index==0)
        {
            i=0;
            j=0; //compare first letter
        }
    if(index==1)
        {
            i=1;
            j=1;
        }
    if(index==3)
        {
            i=word_len-1;//last letter of input word
        }
   while(low <= high) //if word is too short will automatically return -1
   {
        mid = (low + high) / 2;
        j=Find_Word_Length(dic[mid])-1; //last letter of current dictionary word
        if(Upper_Or_Lower(w[i])!=0) // not upper
            {
                if (w[i] < (dic[mid])[j]) //comparison of letters
                    high = mid - 1;
                else if (w[i] > (dic[mid])[j])
                 low = mid + 1;
               else //if a match was found
                 return mid; //return index of the matching word
            }
        else //is upper
            {
                if (w[i]+alphabet < (dic[mid])[j]) //comparison of letters
                    high = mid - 1;
                else if (w[i]+alphabet > (dic[mid])[j])
                 low = mid + 1;
               else //if a match was found
                 return mid; //return index of the matching word

            }
   }
   return -1;
}



/*
char Find_Last_Letter (char *word) //FUNC NOT NEEDED
    {
        int length;
        length=Find_Word_Length(word);
        return word[length-1];
    }
*/

/*
int main()
    {
        //char *wordList[len]={"bad","diff","do","farm","feel","in","luck","play","shift"};
        char dicword[]="maoroy";
        //char searchword[]="roy";
        //char s[30]; //this will hold the lowercased input words for the checks
        //applyFilter1(searchword);
        char a;
        a=Find_Last_Letter(dicword);
        printf("val is %c",a);
    }
*/


int matchWordInList1(char *wordList[], int wordListLength, char *wordToSearch) //DONT CHANGE THE SIGNATURE
    {
        int res1;
        char *newList[2];
        res1=binarySearch(wordList, wordListLength, wordToSearch, 0);
        if(res1==-1) //didnt find a first letter match
            return 0;
        else if ((wordToSearch[0]!='f' || wordToSearch[0]!='F') && (wordToSearch[0]!='d' || wordToSearch[0]!='D')) //found 1st letter match and 1st letter isnt f or b
          {
                if(Is_Exact_Or_Prefix(wordToSearch,wordList[res1]))
                    return 1; //found match
                else
                    return 0;
          }
        else //if first letter is d or f
            {
                int res2; //1 if d, 4 if f
                if(wordToSearch[0]=='d' || wordToSearch[0]=='D')
                    {
                       newList[0]=wordList[res1];
                       newList[1]=wordList[res1+1];
                    }
                else
                    {
                        newList[0]=wordList[res1-1];
                        newList[1]=wordList[res1];
                    }
                res2=binarySearch(newList, 2, wordToSearch, 1); //this should operate on the 2 matching words only
                if(res2==-1)
                    return 0;
                else
                    {
                        if(Is_Exact_Or_Prefix(wordToSearch,newList[res2]))
                            return 1; //found match
                        else
                            return 0;
                    }
            }
    }



int matchWordInList2(char *wordList[], int wordListLength, char wordToSearch[]) //DONT CHANGE THE SIGNATURE
    {
        if(matchWordInList1(wordList,wordListLength,wordToSearch))
            return 1; //found match by regular terms
        int res1, res2=0; //res1 for LastLetters, res2 for original wordlist
        char* Last_Letters[len]={"d","f","k","l","m","n","o","t","y"};
        res1=binarySearch(Last_Letters,wordListLength,wordToSearch,3);
        if(res1==-1)
                return 0;
        switch(res1)
            {
                case 0: res2=0; break;
                case 1: res2=1; break;
                case 2: res2=6; break;
                case 3: res2=4; break;
                case 4: res2=3; break;
                case 5: res2=5; break;
                case 6: res2=2; break;
                case 7: res2=8; break;
                case 8: res2=7; break;
            }
        for (int i=0; i<wordListLength; i++)
        if (backwords_match(wordToSearch,wordList[res2]))
            return 1;
        return 0;
    }



void changeWord (char *word, int i, int filter_type)
    {
                if(word[i]=='a' || word[i]=='A')
                    word[i]='@';
                else if(word[i]=='e' || word[i]=='E')
                    word[i]='3';
                else if(word[i]=='o' || word[i]=='O')
                    word[i]='0';
                else if(word[i]=='u' || word[i]=='U')
                    word[i]='4';
                else if(word[i]=='i' || word[i]=='I')
                    word[i]='1';
                else if(word[i]!='-' && filter_type==2)
                    word[i]='*';
    }

void applyFilter1(char word[]) ////DONT CHANGE THE SIGNATURErecives word, filters it. called only when there is a match (checked on main)
    {
        int length;
        length=Find_Word_Length(word);
        for(int i=0;i<length;i++)
            {
                changeWord(word,i,1);
            }
        printf("%s",word);
    }


void applyFilter2(char word[]) ////DONT CHANGE THE SIGNATURErecives word, filters it. called only when there is a match (checked on main)
    {
        int length;
        length=Find_Word_Length(word);
        for(int i=0;i<length;i++)
            {
                changeWord(word,i,2);
            }
        printf("%s",word);
    }


void Reset_Array (char* word)
    {
        int length;
        length=Find_Word_Length(word);
        for(int i=0; i<length; i++)
            word[i]=0;
    }


void Print_Word (char** wordList,int length,char* this_word, int flag)
    {
        int did=0;
        switch(flag)
             {
                case 1:
                    if(matchWordInList1(wordList,length,this_word))
                    {
                        did=1;
                        applyFilter1(this_word);
                        Reset_Array(this_word);
                        break;
                    }
                case 2:
                    if(matchWordInList2(wordList,length,this_word))
                        {
                            did=1;
                            applyFilter2(this_word);
                            Reset_Array(this_word);
                            break;
                        }
             }
        if(did==0)
            printf("%s",this_word);
    }

int main()
{
    char *wordList[len]={"bad","diff","do","farm","feel","in","luck","play","shift"};
    char string[31];
    char curr_let, prev_let=0;
    int i=0, filter=0;
    scanf("%c",&curr_let); //deal with first word outside so we can use "previous letter"
    if(curr_let=='1')
        filter=1;
    else if(curr_let=='2')
        filter=2;
    else
        filter=0;
    while(scanf("%c",&curr_let)!=EOF) //begin kelet
        {
                if(prev_let==10) //if first char in row
                    {
                        if(curr_let=='1')
                            filter=1;
                        else if(curr_let=='2')
                            filter=2;
                        else
                            filter=0;
                        i=0;
                    }
                while(filter==0 && curr_let!=10)
                        scanf("%c",&curr_let);
                if(filter==0)
                        {
                            printf("Illegal filter type");
                            i=0;
                        }
                if (curr_let==10) //if we are finishing a row
                    {
                        if(i>0)
                            {
                                i++;
                                string[i]=0; //finish string
                                Print_Word(wordList,len,string,filter);
                                Reset_Array(string);
                                i=0; //for new word to come
                            }
                    }
                else if(Upper_Or_Lower(curr_let)==3) //if tav
                    {
                        if(Upper_Or_Lower(prev_let)==2) //if prev is hyphen
                            {
                                printf("-"); //situation like "abcd-&" word is abcd
                            }
                        else if(Upper_Or_Lower(prev_let)<2) //if like abgfd&
                            {
                                string[i]=0; //finish string
                                Print_Word(wordList,len,string,filter);
                                Reset_Array(string);
                            }
                        i=0; //for new word to come
                        printf("%c",curr_let); //printed anyway
                    }
                else if(Upper_Or_Lower(curr_let)<2) //if is letter
                    {
                        if(prev_let=='-')
                            {
                                string[i]='-';
                                i++;
                            }
                        string[i]=curr_let; //we have - in a middle of a word
                        i++;
                    }
                else //if hyphen
                    {
                        if(prev_let=='-' && i>0) //we have a word stored, like abc--
                            {
                                Print_Word(wordList,len,string,filter);
                                Reset_Array(string);
                                i=0;
                            }
                        if(Upper_Or_Lower(prev_let)==3 || (prev_let=='-' && i==0)) //cuz hyphen cant start a word
                            printf("-");
                    }
            prev_let=curr_let;
        }
    return 0;
}
