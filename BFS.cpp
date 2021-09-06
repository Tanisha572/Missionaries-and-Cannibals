#include<bits/stdc++.h>
using namespace std;

class State{
	int c;
	int m;
	bool isRight;
	
	public:
	//default constructor
	State(){
		c = m = 0;
		isRight = false;
	}
	
	//parameterised constructor with initialization list. 
	//here state is assumed to be valid, therefore, no further checks required.
	State(int sm, int sc, bool sIsRight): isRight(sIsRight), c(sc), m(sm){}
	
	//no setters required as parameters will be set only through constructors
	//getters
	int getCannibals() const{
		return c;
	}
	int getMissionaries() const{
		return m;
	}
	bool getIsRight() const{
		return isRight;
	}
	
	
	// == and != operators are overloaded as they are used by == and != of Config
    bool operator==(const State& s) const{
        return (c==s.c && m==s.m && isRight == s.isRight);
    }
    bool operator!=(const State& s) const{
        return (c!=s.c || m!=s.m || isRight != s.isRight);
    }
    
	
	friend bool isValid(State s, int n);
	friend bool isFinal(State s, int n);
	friend ostream& operator<<(ostream& os, const State& s);
};

ostream& operator<<(ostream& os, const State& s){
	os<<"< "<<s.m<<", "<<s.c<<", ";
	if(s.isRight)
		os<<"right >";
	else
		os<<"left >";
	return os;
}

//validity check
bool isValid(State s, int n){
	return (s.c<=n && s.c>=0 && s.m<=n && s.m>=0 &&(s.c<=s.m || s.m == 0));
}

bool isFinal(State s, int n){
	return (s.c == n && s.m == n && s.isRight);
}
	
	
class Config{
	State left;
	State right;
	public:
	//default constructor
	Config(){}
	//parameterised constructor
	Config(State l, State r): left(l), right(r){}
	
	//getters
	State getLeft() const{
		return left;
	}
	State getRight() const{
		return right;
	}
	
	// == and != operators are overloaded as map uses these to resolve collisions in hash value
	bool operator==(const Config& config) const{
        return (left == config.left && right ==config.right);
    }
    bool operator!=(const Config& config) const{
        return (left != config.left || right != config.right);
    }

	friend ostream& operator<<(ostream& os, const Config& config);

};
ostream& operator<<(ostream& os, const Config& config){
    os<<config.left<<"\t"<<config.right;
    return os;
}

class MyHashFunction {
	public:
  
    // We use userdfined hash functions to get hash value of States.
    // and define our hash function as XOR of the
    // hash values.
    size_t operator()(const Config& config) const{
        return (myHash(config.getLeft()) ^ myHash(config.getRight()));
    }
    
    //our hash function uses pre-defined hash values for int
    int myHash(const State& s) const{
    	return ((hash<int>()(s.getMissionaries()))^(hash<int>()(s.getCannibals()))^(hash<int>()(s.getIsRight())));
	}
    
};



//BFS approach
bool solveMC_BFS(int ml, int cl, int mr, int cr, int n, int boatCapacity, stack<Config >& sol){
	
	vector<Config > visited;
	queue<Config> bfs_q;
	unordered_map<Config, Config, MyHashFunction > halfState;
	unordered_map<Config, Config, MyHashFunction > parentOf;
	
	State initial_sl(ml, cl, false);
	State initial_sr(mr, cr, true);
	
	Config initialConfig(initial_sl,initial_sr);
	bfs_q.push(initialConfig);
	
	
	visited.push_back(initialConfig);
		
	bool isSolved = false;
	
	while(!bfs_q.empty() && !isSolved){
		
		
		Config currentConfig(bfs_q.front());
		bfs_q.pop();
		
		int current_ml = currentConfig.getLeft().getMissionaries();
		int current_cl = currentConfig.getLeft().getCannibals();
		int current_mr = currentConfig.getRight().getMissionaries();
		int current_cr = currentConfig.getRight().getCannibals();
		
		for(int mtr=0; mtr<=boatCapacity; mtr++){
			for(int ctr=0; ctr<=boatCapacity; ctr++){
				if((mtr == 0 && ctr == 0) || (mtr+ctr>boatCapacity))
					continue;
				int half_ml = current_ml-mtr;
				int half_cl = current_cl-ctr;
				int half_mr = current_mr+mtr;
				int half_cr = current_cr+ctr;
				
				State half_sl(half_ml, half_cl, false);
				State half_sr(half_mr, half_cr, true);
				
				
				
				if(!(isValid(half_sl, n) && isValid(half_sr, n)))
					continue;
					
				Config halfConfig(half_sl, half_sr);
				
				if(isFinal(half_sr, n)){

					sol.push(halfConfig);
					parentOf[halfConfig] = currentConfig;
					//constructing the solution stack
					sol.push(parentOf[halfConfig]);
					Config x = sol.top();
					while(x != initialConfig){
						sol.push(halfState[x]);
						sol.push(parentOf[x]);
						x = parentOf[x];
					}
					isSolved = true;
					break;
				}
				
				for(int mtl=0; mtl<=boatCapacity; mtl++){
					for(int ctl=0; ctl<=boatCapacity; ctl++){
						if((mtl == mtr && ctl == ctr) || (mtl == 0 && ctl == 0) || (mtl+ctl>boatCapacity))
							continue;
						int full_ml = half_ml+mtl;
						int full_cl = half_cl+ctl;
						int full_mr = half_mr-mtl;
						int full_cr = half_cr-ctl;
						
						State full_sl(full_ml, full_cl, false);
						State full_sr(full_mr, full_cr, true);
						
						if(!(isValid(full_sl, n) && isValid(full_sr, n)))
							continue;
						else{
							Config fullConfig(full_sl, full_sr);
							if(find(visited.begin(), visited.end(),fullConfig) != visited.end())
								continue;
							else{

								visited.push_back(fullConfig);
								bfs_q.push(fullConfig);
								halfState.insert({fullConfig, halfConfig});
								parentOf.insert({fullConfig, currentConfig});
								
							}
						}
						
					}
				}
			}
			if(isSolved)
				break;
		}
	}
	if(isSolved)
		return true;
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
		
		
		stack<Config>sol;
		
		
		//use BFS to travel through the search space and use visited to check if a certain state has already been visited
		bool isSolved = false;
		//with boat capacity 1, we can never progress towards the solution. 
		//taking the boat to the right will immediately require the person, who just crossed, to take the boat back to the left bank, therefore, making no progress beyond the initial configuration
		if(boatCapacity != 1)
			isSolved = solveMC_BFS(n, n, 0, 0, n, boatCapacity, sol);
		if(isSolved){
			//puzzle successfully solved!
			cout<<"\t\tpuzzle successfully solved!\t\n\n";
			//inserting initial state
			while(!sol.empty()){
				cout<<"\t";
				Config c = sol.top();
				sol.pop();
				cout<<c<<endl;
			}

		}
		else
			cout<<"FAILED\n";
		cout<<"\n\tcontinue? (y/n): ";
		cin>>c;
		
	}while(c == 'y' || c == 'Y');
	
	return 0;
}
