/*
 TOE1 - Tic-Tac-Toe ( I )
#game

Tic Tac Toe is a child's game played on a 3 by 3 grid. One player, X, starts by placing an X at an unoccupied grid position. Then the other player, O, places an O at an unoccupied grid position. Play alternates between X and O until the grid is filled or one player's symbols occupy an entire line (vertical, horizontal, or diagonal) in the grid.


We will denote the initial empty Tic Tac Toe grid with nine dots. Whenever X or O plays we fill in an X or an O in the appropriate position. The example below illustrates each grid configuration from the beginning to the end of a game in which X wins.



subir imagenes


Your job is to read a grid and to determine whether or not it could possibly be part of a valid Tic Tac Toe game. That is, is there a series of plays that can yield this grid somewhere between the start and end of the game?



Input

The first line of input contains N, the number of test cases. 4N-1 lines follow, specifying N grid configurations separated by empty lines. 

Output

For each case print "yes" or "no" on a line by itself, indicating whether or not the configuration could be part of a Tic Tac Toe game.

Example

Input:
2
X.O
OO.
XXX

O.X
XX.
OOO

Output:
yes
no
 */

#include <bits/stdc++.h>
#define ll					long long
#define MOD					1000000007
#define MAX					1e9
#define MIN                 -1e9
#define PI					acos(-1.0)
#define vi					vector<int>
#define vii					vector<vi>
#define vll					vector<ll>
#define mid(s,e)			(s+(e-s)/2)

using namespace std;

void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}

template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
template<typename T>
void printVec(const T &v, char sep=' ') {for (auto &i: v) cout << i << sep;}
template<typename T>
void printVecPair(const T &v, char sep=' ') {for (auto &i: v) cout << i.first <<" " << i.second<< sep;}


#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif
bool vecEq(vector<string>&a , vector<string>&b) {
		for(int i=0; i<3;i++) {
				for(int j=0; j<3;j++) {
						if(a[i][j] != b[i][j]) return false;
				}
		}
		return true;

}
bool isWin(vector<string>& cur) {
		bool row = false;
		for(int i=0;i<3;i++) {
			 row = (row || (cur[i][0] == cur[i][1] && cur[i][1] == cur[i][2] && cur[i][2] != '.'));	
		}
		bool col = false;
		for(int i=0;i<3;i++) {
			 col = (col || (cur[0][i] == cur[1][i] && cur[1][i] == cur[2][i] && cur[2][i] != '.'));	
		}
		bool dia = false;
		dia = (dia || (cur[0][0] == cur[1][1] && cur[1][1] == cur[2][2] && cur[2][2] != '.'));	
		dia = (dia || (cur[0][2] == cur[1][1] && cur[1][1] == cur[2][0] && cur[2][0] != '.'));	
		return (col || row || dia);
}
int main() {
	ios::sync_with_stdio(false);
 	cin.tie(0);
	int n;
	cin>>n;
	while(n--) {
			vector<string> grid(3);
			for(int i=0;i<3;i++) cin>>grid[i];
			vector<string> first = {"...","...","..."};
			string ans = "no";
			queue<pair<vector<string>,bool>> q;
			q.push({first,1});
			while(!q.empty()) {
					auto p = q.front();
					vector<string> cur = p.first;
					bool player = p.second;
					q.pop();
					if(vecEq(grid,cur)) {
							ans = "yes";
							break;
					}
					if(isWin(cur)) continue;
					for(int i=0;i<3;i++) {
							for(int j=0; j<3;j++) {
									if(cur[i][j] == '.') {
											cur[i][j] = (player ? 'X' : 'O');
											if(cur[i][j] == grid[i][j]) {
													q.push({cur,1 ^ player});
											}
											cur[i][j] = '.';

									}


							}
					}

			}
			cout<<ans<<"\n";
					
	}
	return 0;
}



