#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
using namespace std;
vector<vector<string> > vec;
string s;
vector<string> vs;
int main(){
	cin>>s;
	vs.push_back(s);
	vec.push_back(vs);
	cout<<vec[0][0]<<endl;
	cout<<vec[0][0][0]<<endl;
	return 0;
}
