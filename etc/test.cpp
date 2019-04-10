#include <iostream>
#include <vector>
#include <string>
#include <string.h>

    bool isOneOfTheFollowing( char c, char *among );

    int splitStringIntoVector( std::string& s, std::vector<std::string>& splitted, const char *delimiters );


    int splitStringIntoVector( std::string& s, std::vector<std::string>& splitted, const char *delimiters )
    {
        int len = s.length();
        int start = 0;
        bool started = true;
        int numSplitted=0;

        int i = 0;
        for(  ; i < len ; i++ ) {
          if( !isOneOfTheFollowing( s[i], (char*)delimiters ) ) {
            start = i;
            started = true;
            break;
          }
        }
        if( !started ) {
          return -1;
        }

        for( ; i < len ; i++ ) {
          if( isOneOfTheFollowing( s[i], (char*)delimiters ) ) {
            splitted.push_back( s.substr( start, i-start ) );
            numSplitted++;
            started = false;
            for( int j=i+1 ; j < len ; j++ ) {
              if( !isOneOfTheFollowing( s[j], (char*)delimiters ) ) {
                i = j;
                start = j;
                started = true;
                break;
              }
            }
            if( !started ) {
             break;
            }
          }
        }
        if( started ) {
            splitted.push_back( s.substr( start, len-1 ) );          
        }

      return numSplitted;
    }

    bool isOneOfTheFollowing( char c, char *among ) {
      int len = strlen(among);
      for( int i = 0 ; i < len ; i++ ) {
        if( c == among[i] ) {
          return true;
        }
      }
      return false;
    }

int main()
{
  std::string s("111\r111\n11123\r\n1312313  1323123123 123213 123 \r\n 123213 werewrwer");  
  std::vector<std::string> v;
  int n=splitStringIntoVector( s, v, " \r\n" );

  std::cout << "n=" << n << std::endl;     
  for( int i = 0 ; i < v.size() ; i++ ) {
    std::cout << v[i] << std::endl; 
  }
   
   return 0;
} 