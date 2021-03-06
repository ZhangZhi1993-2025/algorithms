#include <iostream>
#include <set>
#include <stack>
#include <math.h>
using namespace std;

struct SubStr {
    int x;
    int y;
    string str;
    SubStr ( int xx, int yy, string s ) : x ( xx ), y ( yy ), str ( s ) {}
    SubStr() {}
};

// do not use condition compression and create a dp array
set<string> longestCommonSubSequence1 ( string a, string b ) {
    //initialize dp array
    int **dp = new int*[a.size()];
    for ( unsigned int i = 0; i < a.size(); i++ )
        dp[i] = new int[b.size()];

    //initialize the begining data in dp array
    for ( unsigned int i = 0; i < a.size(); i++ ) {
        if ( b[0] != a[i] )
            dp[i][0] = 0;
        else {
            for ( ; i < a.size(); i++ )
                dp[i][0] = 1;
            break;
        }
    }
    for ( unsigned int i = 0; i < b.size(); i++ ) {
        if ( a[0] != b[i] )
            dp[0][i] = 0;
        else {
            for ( ; i < b.size(); i++ )
                dp[0][i] = 1;
            break;
        }
    }

    //running the dynamic programming algorithm to find the max length
    for ( unsigned int i = 1; i < a.size(); i++ )
        for ( unsigned int j = 1; j < b.size(); j++ ) {
            if ( a[i] == b[j] )
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] =  dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
        }

    //find the sub sequence
    set<string> result;
    string str_tmp = "";
    stack<SubStr> tmp; //temporary intermediate stack
    SubStr s;
    int x = a.size() - 1;
    int y = b.size() - 1;
    while ( !tmp.empty() || ( x > -1 && y > -1 ) ) {
        if ( x > 0 || y > 0 ) {
            // go to the top-left
            if ( x > 0 && y > 0 && dp[x][y] > dp[x - 1][y] && dp[x][y] > dp[x][y - 1] ) {
                str_tmp = a[x] + str_tmp;
                x--;
                y--;
            }
            // go to the top
            else if ( x > 0 && dp[x][y] == dp[x - 1][y] ) {
                if ( dp[x][y] == dp[x][y - 1] )
                    tmp.push ( SubStr ( x, y - 1, str_tmp ) );
                x--;
            }
            // go to the left
            else if ( y > 0 && dp[x][y] == dp[x][y - 1] )
                y--;
        }
        // comes to the last dp[0][0]
        else if ( x == 0 && y == 0 ) {
            if ( a[0] == b[0] )
                str_tmp = a[0] + str_tmp;
            x--;
            y--;
        }
        // the current is over and pop the next
        else {
            result.insert ( str_tmp );
            s = tmp.top();
            tmp.pop();
            x = s.x;
            y = s.y;
            str_tmp = s.str;
        }
    }

    for ( unsigned int i = 0; i < a.size(); i++ )
        delete dp[i];
    delete[] dp;

    return result;
}

// use condition compression and only return the length of the common sub sequence
int longestCommonSubSequence2 ( string a, string b ) {
    // the iteration intermediate varialbe
    int top_left = 0;
    int top = 0;
    int current = 0;

    // make sure a is longer than b
    if ( a.size() < b.size() ) {
        string tmp;
        tmp = a;
        a = b;
        b = tmp;
    }

    // only cost O(min(M,N)) space
    int *dp = new int[b.size()];
    // judge if dp[0] == 1
    bool isOne = false;

    // initialize dp array
    dp[0] = a[0] == b[0] ? 1 : 0;
    isOne = a[0] == b[0] ? true : false;
    for ( unsigned int i = 1; i < b.size(); i++ ) {
        if ( dp[i - 1] == 1 || a[0] == b[i] )
            dp[i] = 1;
        else
            dp[i] = 0;
    }

    // run the dynamic programming algorithm
    for ( unsigned int i = 1; i < a.size(); i++ )
        for ( unsigned int j = 0; j < b.size(); j++ ) {
            top = dp[j];
            current = dp[j];
            if ( j == 0 ) {
                if ( isOne == true )
                    continue;
                if ( a[i] == b[j] )
                    dp[j] = 1;
                else
                    dp[j] = 0;
            } else {
                if ( a[i] == b[j] )
                    dp[j] = top_left + 1;
                else
                    dp[j] = top > dp[j - 1] ? top : dp[j - 1];
            }
            top_left = current;
        }

    int result = dp[b.size() - 1];
    delete dp;

    return result;
}

int main () {
    set<string> s = longestCommonSubSequence1 ( "acbac", "abdca" );
    for ( set<string>::iterator iter = s.begin(); iter != s.end(); iter++ )
        cout << *iter << endl;
    cout << endl << longestCommonSubSequence2 ( "abcac", "adbca" ) << endl;
    return 0;
}
