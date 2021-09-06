#include<bits/stdc++.h>
using namespace std;

struct State{
	int c;
	int m;
	bool isRight;
	
	//default constructor
	State(){
		c = m = 3;
		isRight = false;
	}
	
	//parameterised constructor with initialization list. 
	//here state is assumed to be valid, therefore, no further checks required.
	State(int sm, int sc, bool sIsRight): isRight(sIsRight), c(sc), m(sm){}
	
	//print state
	void printState(){
		cout<<"< "<<this->m<<", "<<this->c<<", ";
		if(this->isRight)
			cout<<"right >";
		else
			cout<<"left >";
	}
	
	//overloading operator < as set from stl will need to compare states and State is user-defined. if not overloaded then < will only check for address which will always be different
	//Elements a and b are considered equal iff !(a < b) && !(b < a)
	// This function is used by set to order
    // elements of Test.
    bool operator==(const State& s) const
    {
        return (c==s.c && m==s.m && isRight == s.isRight);
    }
	
	friend bool isValid(State* s, int n);
	friend bool isFinal(State* s, int n);
};

//validity check
bool isValid(State s, int n){
	return (s.c<=n && s.c>=0 && s.m<=n && s.m>=0 &&(s.c<=s.m || s.m == 0));
}

bool isFinal(State s, int n){
	return (s.c == n && s.m == n && s.isRight);
}


bool solveMC_DFS(int ml, int cl, int mr, int cr, int n, int boatCapacity, stack<pair<State, State>>& sol, vector<pair<State, State>>& visited){
	State sl(ml, cl, false);
	State sr(mr, cr, true);
	
	//checking validity of the current states <ml,cl,0> and <mr,cr,1> 
	if(!(isValid(sl, n) && isValid(sr, n)))
		return false;
			
	//if this configuration is already achieved and is still in the array, then we can't visit again as then it'll cause a cycle.
	if(find(visited.begin(), visited.end(), make_pair(sl, sr)) == visited.end()){
		visited.push_back(make_pair(sl,sr));
	}
	else{
		return false;
	}
	
	//base case: only 1 missionary and 1 cannibal
	if(n == 1){
		//in one move, final state is reached. therefore, push it in the vector and exit
		State final_sl(0, 0, false);
		State final_sr(n, n, true);
		sol.push(make_pair(final_sl,final_sr));		
		return true;
	}
	
	//if final state is reached, control returns from inside the loop itself so no need to check here
	
	
	/*	mtr - missionaries to move to right
		ctr - cannibals to move to right
		mtl - missionaries to move to left
		ctl - cannibals to move to left
	
	*/
	for(int mtr=0; mtr<=boatCapacity; mtr++){
		for(int ctr=0; ctr<=boatCapacity; ctr++){
			if((mtr == 0 && ctr == 0) || (mtr+ctr>boatCapacity) )
				continue;
			int half_ml = ml-mtr;
			int half_cl = cl-ctr;
			int half_mr = mr+mtr;
			int half_cr = cr+ctr;
			
			State half_sl(half_ml, half_cl, false);
			State half_sr(half_mr, half_cr, true);
			
					
			if(!(isValid(half_sl, n) && isValid(half_sr, n))){
				continue;
			}
			if(isFinal(half_sr, n)){
				sol.push(make_pair(half_sl,half_sr));
				return true;
			}
			
			for(int mtl=0; mtl<=boatCapacity; mtl++){
				for(int ctl=0; ctl<=boatCapacity; ctl++){
					
					//transporting the same ppl will make net movemement in current trip 0, also it is compulsory for some movement to right to left if something was done from left to right
					//therefore, don't allow these trips
					if((mtr == mtl && ctr == ctl) || (mtl == 0 && ctl == 0) || (mtl+ctl>boatCapacity)){
						continue;
					}
					int full_ml = half_ml+mtl;
					int full_cl = half_cl+ctl;
					int full_mr = half_mr-mtl;
					int full_cr = half_cr-ctl;
					
					State full_sl(full_ml, full_cl, false);
					State full_sr(full_mr, full_cr, true);
					
					if(!(isValid(full_sl, n) && isValid(full_sr, n))){
						continue;
					}
					else{

						if(find(visited.begin(), visited.end(), make_pair(full_sl, full_sr)) != visited.end())
							continue;
						if(!solveMC_DFS(full_ml, full_cl, full_mr, full_cr, n, boatCapacity, sol, visited)){
							continue;
						}
						else{
							sol.push(make_pair(full_sl,full_sr));
							sol.push(make_pair(half_sl,half_sr));
							//updating the states iff the current state is known to lead to solution
							return true;
						}
					}
				}
			
			}	
		}
	}
	//any round trips from this configuration doesn't lead to ans
	return false;
}

int main(){
	
	int n, boatCapacity;
	char c;
	cout<<"\t\t\t\tMISSIONARIES AND CANNIBALS\t\t\t\n\n";
	cout<<"\t\t\t\t---------------------------\n";
	do{
		
		cout<<"\n\tenter no. of missionaries: ";
		cin>>n;
		cout<<"\tenter boat capacity: ";
		cin>>boatCapacity;
		cout<<"\n\n";
		
		
		
		stack<pair<State, State>> sol;
		vector<pair<State, State>> visited;
		//use DFS to travel through the search space and use visited to check if a certain state has already been visited
		bool isSolved = false;
		if(boatCapacity != 1)
			isSolved =solveMC_DFS(n, n, 0, 0, n, boatCapacity, sol, visited);
		if(isSolved){
			//puzzle successfully solved!
			cout<<"\t\tpuzzle successfully solved!\t\n\n";
			//inserting initial state
			State isl(n,n,false);
			State isr(0,0,true);
			sol.push(make_pair(isl, isr));
			while(!sol.empty()){
				pair<State, State> p = sol.top();
				sol.pop();
				cout<<"\t\t";
				p.first.printState();
				cout<<"\t\t";
				p.second.printState();
				cout<<endl;
			}

		}
		else{
			cout<<"FAILED\n";
		}
		cout<<"\n\tcontinue? (y/n): ";
		cin>>c;
		
	}while(c == 'y' || c == 'Y');
	
	return 0;
}
