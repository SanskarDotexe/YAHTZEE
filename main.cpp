
/***********************************************************
Program 5:YAHTZEE Project

Course: CS 141, Fall 2022, UIC

System: CLION on Windows 11

Author: Sanskar Kalal(651632119), starter code provided by Sara Riazi
 **********************************************************/

#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include "vector"
#include <cstring>
#include <limits>

using namespace std;



//#################################################################################################


// Dice Class 
class Dice{

    // Has a Value and A status that indicates whether it should be rolled or kept in hand
private:
    bool keep;
    int value;

public:
    // Default Dice Constructor
    Dice(){
        // all dice can be rolled
        keep = false;
    }
    // function to change to status of the dice
    void setKeep(bool keep){
        this->keep = keep;
    }
    // function to get the status of the dice
    bool getKeep(){
        return keep;
    }

    //Randomly assigns a value between from 1 to 6 to the dice.
    void roll(){
        this->value = (rand()%6)+1;
    };

    //reveals the value of the dice
    int reveal() {return value;}

    //The autograder will set the values directly instead of calling roll() to avoid randomness!
    void setValue(int value) {this->value = value;}

};


//#################################################################################################

const int HAND_SIZE = 5;

// Hand Class
class Hand {
public:

    //Function Displays the values of the five dice
    void show(){
        cout<<"Hand: ";
        for(int i =0;i<HAND_SIZE;i++){
            cout<<this->dice[i].reveal()<<" ";

        }
        cout<<endl;

    };

    // Rolls the dice that are not kept by the player
    void play(){
        // check all the dice and see if they are to be kept in hand
        for (int i=0;i<HAND_SIZE;i++){
            if(dice[i].getKeep()){
                // don't roll if they are to be kept
                continue;
            }else{
                // roll the rest
                dice[i].roll();
            }

        }
        for (int i=0;i<HAND_SIZE;i++) {
            //reset the status for all once they are rolled
            dice[i].setKeep(false);
        }

    };

    // function to get the pointer to the dice
    Dice* getDice(int diceNum){
        return &dice[diceNum];
    };

    //selection is the string of dice numbers that the player wants to keep.
    //For example, "125" means that player wants to keep the first, second, and fifth dice, and roll the rest.
    void setSelection(string selection){
        string STRING = "123456";
        int num;
        // set the dice status for the selected dice
        for(int i = 0;i<selection.size();i++){
            num = selection[i]-48;
            dice[num-1].setKeep(true);
        }

    };

    //Default constructor for the Hand
    Hand(){
        //Roll all the dice initially
        for(int i = 0;i<HAND_SIZE;i++){
            dice[i].roll();
        }
    };

private:
    Dice dice[HAND_SIZE]; //an array of five dice

};



//######################################################################


//List of rows in the board
const int ONES = 1;
const int TWOS = 2;
const int THREES = 3;
const int FOURS = 4;
const int FIVES = 5;
const int SIXES = 6;
const int THREE_OF_KIND = 7;
const int FOUR_OF_KIND = 8;
const int FULL_HOUSE = 9;
const int SMALL_STRAIGHT = 10;
const int LARGE_STRAIGHT = 11;
const int CHANCE = 12;
const int YAHTZEE = 13;

const int BOARD_SIZE=13;

class Game {

public:
    //calcScore returns a score of a hand (5 dice) for given row in the board.
    //Note the rows are indexed from zero.
    //For example if the hand is 2 1 1 5 1 then calcScore(hand, ONES) returns 3 and  calcScore(hand, TWOS) returns 2.
    int calcScore(Hand* hand, int row){

        // set a counter of all the dice numbers
        vector<int> counter(6);
        for(int i=0;i<counter.size();i++){
            counter.at(i) = 0;
        }

        //count each number on the dices and add to the counter
        for(int i =0;i<HAND_SIZE;i++){
            if(hand->getDice(i)->reveal()==1){
                counter.at(0) += 1;
            }
            if(hand->getDice(i)->reveal()==2){
                counter.at(1) += 1;
            }
            if(hand->getDice(i)->reveal()==3){
                counter.at(2) += 1;
            }
            if(hand->getDice(i)->reveal()==4){
                counter.at(3) += 1;
            }
            if(hand->getDice(i)->reveal()==5){
                counter.at(4) += 1;
            }
            if(hand->getDice(i)->reveal()==6){
                counter.at(5) += 1;
            }

        }

        // return the amount of ONES in the counter
        if(row == 1){
            return counter.at(0);
        }
        // return the sum of all TWOS in the counter
        if(row==2){
            return counter.at(1)*2;
        }
        // return the sum of all THREES in the counter
        if(row==3){
            return counter.at(2)*3;
        }
        // return the sum of all FOURS in the counter
        if(row==4){
            return counter.at(3)*4;
        }
        // return the sum of all FIVES in the counter
        if(row==5){
            return counter.at(4)*5;
        }
        // return the sum of all SIXES in the counter
        if(row==6){
            return counter.at(5)*6;
        }
        // return the sum of all dices if one of the counter is 3 or more
        if(row == 7){
            bool sumAll = false;
            int sumOfAll = 0;
            for (int i = 0; i < counter.size(); i++) {
                if (counter.at(i)>=3){
                    sumAll = true; //take sum if a three or more dice have the same number
                }
            }
            // add all the dice together
            if (sumAll){
                for (int i = 0; i < counter.size(); i++) {
                    sumOfAll += (i+1)*counter.at(i);

                }

            }
            return sumOfAll;
        }
        // return the sum of all dices if one of the counter is 4 or more
        if (row==8){
            bool FourofKind = false;
            int sumOfAllDice = 0;

            for(int i=0;i<counter.size();i++){
                if(counter.at(i)>=4){
                    FourofKind = true;//take sum if a four or more dice have the same number
                }
            }
            // add all the dice together
            if (FourofKind){
                for (int i = 0; i < counter.size(); i++) {
                    sumOfAllDice += (i+1)*counter.at(i);

                }

            }
            return sumOfAllDice;
        }
        // return 25 if there is a pair and a Three of a kind sequence
        if(row==9){
            bool pair =false;
            bool threeOfKind = false;
            // check if there is an exact Three of a kind or a Yahtzee (which includes a Three of a kind)
            for(int i=0;i<counter.size();i++){
                if(counter.at(i)==3 || counter.at(i)==5){
                    threeOfKind = true;
                }
                //check for an exact pair or a Yahtzee (which includes a pair)
                if(counter.at(i)==2 || counter.at(i)==5){
                    pair =true;
                }
            }
            // return 25 if both conditions are met
            if(pair && threeOfKind){
                return 25;
            }
            else{return 0;}
        }
        // return 30 if counter gives one of each number four times in a row (which means it has a sequence of 4 continuos numbers)
        if(row == 10){
            int countOnes = 0;
            for(int i =0;i<counter.size();i++){
                if (counter.at(i)>=1){
                    countOnes+=1; //count the number of single numbers
                    if (countOnes>=4){
                        return 30; // return 30 if you get four single digits in a row
                    }
                } else{
                    countOnes = 0; // reset the counter if a number in between is missing
                }
            }
            return 0;
        }
        // return 25 if counter gives one of each number five times in a row(which gives a sequence of 5 continuos numbers)
        if (row ==11){
            int largeCount=0;
            for(int i =0;i<counter.size();i++){
                if (counter.at(i)>=1){
                    largeCount+=1; //count the number of single numbers
                    if (largeCount==5){
                        return 40;// return 40 if you get five single digits in a row
                    }
                } else{
                    largeCount = 0;// reset the counter if a number in between is missing
                }
            }
            return 0;
        }
        //return the sum of all dices
        if (row ==12){
            int chanceSum=0;
            for(int i=0;i<counter.size();i++){
                chanceSum += (i+1)*counter.at(i);

            }

            return chanceSum;

        }
        // return 50 if any counter is set to Five (means all dice have the same number)
        if (row==13){
            for(int i =0;i<counter.size();i++){
                if(counter.at(i)==5){
                    return 50;
                }
            }
            return 0;
        }

        return 0;

    };


//    Display the board
    void show(){
        cout<<"1. Ones: "<<setw(25)<<scores.at(0)<<endl;
        cout<<"2. Twos: "<<setw(25)<<scores.at(1)<<endl;
        cout<<"3. Threes: "<<setw(23)<<scores.at(2)<<endl;
        cout<<"4. Fours "<<setw(25)<<scores.at(3)<<endl;
        cout<<"5. Fives "<<setw(25)<<scores.at(4)<<endl;
        cout<<"6. Sixes "<<setw(25)<<scores.at(5)<<endl;
        cout<<"Sum: "<<setw(29)<<getUpperScore()<<endl;
        cout<<"Bonus: "<<setw(27)<<getBonusScore()<<endl;
        cout<<"7. Three of a kind: "<<setw(14)<<scores.at(6)<<endl;
        cout<<"8. Four of a kind: "<<setw(15)<<scores.at(7)<<endl;
        cout<<"9. Full House: "<<setw(19)<<scores.at(8)<<endl;
        cout<<"10. Small straight: "<<setw(14)<<scores.at(9)<<endl;
        cout<<"11. Large straight: "<<setw(14)<<scores.at(10)<<endl;
        cout<<"12. Chance"<<setw(24)<<scores.at(11)<<endl;
        cout<<"13. Yahtzee: "<<setw(21)<<scores.at(12)<<endl;
        cout<<"Total: "<<setw(27)<<getTotalScore()<<endl;

    };


    //Returns the score of the upper part of the board
    int getUpperScore(){
        int sum = 0;
        for(int i=0;i<6;i++){
            if(scores.at(i)!="-"){ // add all the rows that have been played
                sum+=stoi(scores.at(i)); //change string to int and add
            }
        }
        return sum;
    };

    //Returns the score of the lower part of the board
    int getLowerScore(){
        int lowerSum = 0;
        for(int i=6;i<13;i++){
            if(scores.at(i)!="-"){
                lowerSum+=stoi(scores.at(i)); //change string to int and add
            }

        }
        return lowerSum;

    };


    //Returns the bonus points
    int getBonusScore(){
        // if the upper score is 63 or more then add the bonus
        if (getUpperScore()>=63){
            return 35;
        }
        else{return 0;}
    };

    //Returns the total score
    int getTotalScore(){return (getUpperScore()+getLowerScore()+getBonusScore());};

    //Play a hand based on the selected row
    void play(Hand* hand, int row){
        // set the scores for each input and mark the rows played
        if(row == 1){
            scores.at(0) = to_string(calcScore(hand,ONES));
            played.at(1) = true;
        }
        if(row==2){
            scores.at(1) = to_string(calcScore(hand,TWOS));
            played.at(2) = true;
        }
        if(row==3){
            scores.at(2) = to_string(calcScore(hand,THREES));
            played.at(3) = true;
        }
        if(row==4){
            scores.at(3) = to_string(calcScore(hand,FOURS));
            played.at(4) = true;
        }
        if(row==5){
            scores.at(4) = to_string(calcScore(hand,FIVES));
            played.at(5) = true;
        }
        if(row==6){
            scores.at(5) = to_string(calcScore(hand,SIXES));
            played.at(6) = true;
        }
        if(row==7){
            scores.at(6) = to_string(calcScore(hand,THREE_OF_KIND));
            played.at(7) = true;
        }
        if(row==8){
            scores.at(7) = to_string(calcScore(hand,FOUR_OF_KIND));
            played.at(8) = true;
        }
        if(row==9){
            scores.at(8) = to_string(calcScore(hand,FULL_HOUSE));
            played.at(9) = true;
        }
        if(row==10){
            scores.at(9) = to_string(calcScore(hand,SMALL_STRAIGHT));
            played.at(10) = true;
        }
        if(row==11){
            scores.at(10) = to_string(calcScore(hand,LARGE_STRAIGHT));
            played.at(11) = true;
        }
        if (row==12){
            scores.at(11)= to_string(calcScore(hand,CHANCE));
            played.at(12) = true;
        }
        if (row==13){
            scores.at(12) = to_string(calcScore(hand,YAHTZEE));
            played.at(13)=true;
        }


    };


    //Check to see if a row has been played or not (returns true if a row has been played)
    bool isPlayed(int row){
        return played.at(row);
    };

    //Check to see if all rows haven been played or not (returns true if all rows have been played)
    bool isFinished(){
        for(int i =1;i<BOARD_SIZE+1;i++){
            // check if the row is played
            if (!isPlayed(i)){
                return false;
            }
        }
        return true; // true when all rows are played

    };

    Game(){
        // initialize the scores with "-"
        for(int i=0;i<BOARD_SIZE;i++){
            scores.push_back("-");
        }
        // initialize all rows as unplayed
        for(int i=0;i<BOARD_SIZE+1;i++){
            played.push_back(false);

        }
    };


private:
    vector<string> scores; // stores all the scores
    vector<bool> played; // stores the status of each row (played or not)

};




//#######################################################################################

//The run function is the main loop of your program
void run() {
    Game game;
    Hand *hand = new Hand;
    int i = 0;
    int row;
    string selection;

    // run the game till all the rows are played
    while (!game.isFinished()){
        i=0;
        hand->play();
        game.show();
        hand->show();
        // roll two more times after the initial roll
        while (i!=2){
            cout<<"Keep Dice (s to stop rolling): ";
            cin>>selection;
            // stop if "s" is input
            if (selection == "s"){
                break;
            }
            hand->setSelection(selection);
            hand->play();
            i+=1;
            // break if this is the last roll
            if(i>1){
                break;
            }
            hand->show();
        }
        game.show();
        hand->show();
        cout<<"Select a combination to play: ";
        cin>>row;
        // if the row is already played ask for a different input
        while (game.isPlayed(row)){
            cout<<"This combination has been played. Select another one: ";
            cin>>row;
        }
        game.play(hand,row);

    }
    game.show();

}


//For these types of programs there exists many combinations that you might have missed. 
//The best way to check them all is to write test cases for your program. 
//Here I give you an example of checking large straight. You can design your own test cases for different parts.

void test_case() {
    // Game game;
    // Hand* hand = new Hand();
//	hand->getDice(0)->setValue(1);
//	hand->getDice(1)->setValue(1);
//	hand->getDice(2)->setValue(1);
//	hand->getDice(3)->setValue(1);
//	hand->getDice(4)->setValue(1);
//
//	bool check1 = false;
//	if (game.calcScore(hand, ONES) == 5) {
//		check1 = true;
//	}
//	if (check1) {
//		cout << "check 1 passed\n";
//	}
//
//	hand->getDice(0)->setValue(2);
//	hand->getDice(1)->setValue(6);
//	hand->getDice(2)->setValue(2);
//	hand->getDice(3)->setValue(2);
//	hand->getDice(4)->setValue(6);
//
//	bool check2 = false;
//	if (game.calcScore(hand, TWOS) == 6) {
//		check2 = true;
//	}
//
//	if (check2) {
//		cout << "check 2 passed\n";
//	}
//
//	hand->getDice(0)->setValue(6);
//	hand->getDice(1)->setValue(3);
//	hand->getDice(2)->setValue(3);
//	hand->getDice(3)->setValue(6);
//	hand->getDice(4)->setValue(6);
//
//	bool check3 = false;
//	if (game.calcScore(hand, THREES) == 6) {
//		check3 = true;
//	}
//
//	if (check3) {
//		cout << "check 3 passed\n";
//	}
//
//	hand->getDice(0)->setValue(1);
//	hand->getDice(1)->setValue(2);
//	hand->getDice(2)->setValue(3);
//	hand->getDice(3)->setValue(4);
//	hand->getDice(4)->setValue(6);
//
//	bool check4 = false;
//	if (game.calcScore(hand, FOURS) == 4) {
//		check4 = true;
//	}
//
//	if (check4) {
//		cout << "check 4 passed\n";
//	}
}

// THE AUTOGRADER WILL REPLACE THE MAIN FUNCTION. 
// DO NOT ADD ANY LOGIC OF YOUR PROGRAM HERE.
int main() {

    srand(time(0));
    run();
//    test_case();
    return 0;
}