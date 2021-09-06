# Missionaries and Cannibals Problem solved using C++

## <u> Traditional Problem Statement:</u>

In the missionaries and cannibals problem, three missionaries and three cannibals must cross a river using a boat which can carry at most two people, under the constraint that, for both banks, if there are missionaries present on the bank, they cannot be outnumbered by cannibals (if they were, the cannibals would eat the missionaries). The boat cannot cross the river by itself with no people on board.



## <u>Problem tackled:</u>

The original problem is generalised by allowing the player to choose the number of missionaries and cannibals and the boat capacity at the beginning of the game. Other original constraints are kept in place.

Let there be **n** missionaries and cannibals on the left bank and a boat capacity of **b**. 

## <u>Input:</u>

-first line asks for the no. of missionaries of the left bank. 
-second line asks for the boat capacity.

## <u>Output:</u>

-returns the configuration of both banks at every step of the game, until the goal state is reached.

## <u>Constraints:</u>

-  n is a natural number

- b > 1

- for each bank, #missionaries == 0 || (#missionaries => #cannibals) 

## <u>Search Space:</u>

- let a state of any bank of the river be denoted by *<M, C, S>* where, M, C, S is the no. of missionaries, the no. of cannibals and the side of the river respectively. S can either be 0/L (left bank) or 1/R (right bank).
- let at any point of time, the current state of the game be denoted by *<<Ml,Cl,0> <Mr,Cr,1>>* i.e, the left bank and right bank configurations.
- We'll take 1 roundtrip as 1 move. i.e. boat moves with passengers from left to right, and then back to left is considered 1 step in the game. The final state is reached only in half a step. So we'll tackle that separately.

## <u>Goal:</u>

-we'll start with initial configuration *<<n,n,0> <0,0,1>>* and the goal is to traverse through the search space and reach the goal state *<<0,0,0> <n,n,1>>* all the while, maintaining the given constraints.

## <u>Approach:</u>

-The problem can be solved by simulating the game itself.
-The game can be simulated by traversing through the search space and taking the state that moves us closer to the goal state.
-Breath-First Search  and Depth-First Search algorithms can be used for traversing the space.
    -Depth-First Search, although more intuitive approach to reach the goal state, is not the most optimised method as it doesn't guarantee shortest path. 
    -Breath-First Search would always ensure that the goal state is not only reache correctly, adhering to the conditions, but also in minimum no. of moves.

    



  

  

  

