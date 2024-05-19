// timetableGenetics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <fstream>

#define POPULATION 100
#define TOP_POPUL 10
#define MIDDLE_COPIES 5
#define MIDDLE_POPUL (POPULATION - TOP_POPUL) / MIDDLE_COPIES + TOP_POPUL

// Subjects
#define Geography 0
#define English 1
#define Physics 2
#define PSHE 3
#define Drama 4
#define Chemistry 5
#define German 6
#define Latin 7
#define History 8
#define Mathematics 9
#define Biology 10
#define Games 11
#define PE 12
#define Spanish 13
#define Art 14
#define Greek 15
#define Engineering 16
#define Tutors 17
#define ICT 18
#define Nothing 19

using namespace std;
string IDs[20] = { "Geography", "English", "Physics", "PSHE", "Drama", "Chemistry", "German", "Latin", "History", "Mathematics", "Biology", "Games", "PE", "Spanish", "Art", "Greek", "Engineering", "Tutors", "ICT", "" };
static char key;
char nothing[15] = "              ";
enum Day {  
    MonA, TueA, WedA, ThuA, FriA, SatA, SunA,
    MonB, TueB, WedB, ThuB, FriB, SatB, SunB
};
bool compare(char* a, char* b) {
    for (int i = 0; i < max(strlen(a),strlen(b)); i++) {
        if (a[i] != b[i]) { return false; }
        if (a[i] == 0) { return true; }
    }
    return true;
}
class Period {
public:
    int periodID;
    int periodNumber;
    Day day;
    Period() : periodID(Nothing), periodNumber(0), day(MonA) {}
    Period(int ID, int num, Day day) : periodNumber(num), day(day), periodID(ID) {}
};

class Prediction {
public:
    Period periods[154];
    double evaluate(bool debug = (key == 'd')) {
        double total = 0;
        for (int p = 0;p < 154;p++) {
            for (int i = 1;i < 154;i++) {
                if (debug) {
                }
                     if (periods[p].periodID == periods[(p-i < 0) ? (154+p-i) : (p-i)].periodID) {
                    

                    total += sqrt(i);
                    break;
                     }
            }
        }
        return (total);
    }
    string format() {
        string res = "";
        for (int period = 0;period < 11; period++) {
            for (int day = 0;day < 14;day++) {
                string add = IDs[periods[11 * day + period].periodID];
                add.insert(add.begin(), 15 - add.size(), ' ');
                res += add;
            }
            res += "\n";
        }
        return res;
    }
    Prediction(Period prds[154]) {
        copy(prds, prds+154, periods);
    }
    Prediction() {}
};

static int timetable[14][8] = { {0, 1, 2, 3, 4, 5, 6, 7}, {8, 9, 1, 6, 10, 11, 11, 11}, {1, 6, 12, 0, 9, 4, 7, 13}, {5, 2, 14, 14, 10, 9, 1, 15}, {8, 13, 16, 16, 15, 17, 18, 9}, {19, 19, 19, 19, 19, 19, 19, 19}, {19, 19, 19, 19, 19, 19, 19, 19}, {5, 13, 7, 2, 4, 1, 1, 3}, {0, 9, 10, 6, 1, 11, 11, 11}, {7, 6, 12, 1, 9, 4, 13, 0}, {8, 15, 14, 14, 10, 9, 18, 5}, {8, 13, 16, 16, 15, 17, 2, 9}, {19, 19, 19, 19, 19, 19, 19, 19}, {19, 19, 19, 19, 19, 19, 19, 19} };

Prediction* RandomPred() {
    // The only random bit is the last 3 periods - at home
    int toAdd[42] = {
        Mathematics,Mathematics,Mathematics,Mathematics,
        History,History,History,History,
        German,German,German,German,
        Geography,Geography,Geography,Geography,
        Spanish,Spanish,Spanish,Spanish,
        Physics,Physics,Physics,
        Chemistry,Chemistry,Chemistry,Chemistry,
        Latin,Latin,Latin,
        Greek,Greek,Greek,Greek,
        English,English,English,English,
        Biology,Biology,Biology,Biology
    };
    Period revisions[14][3];
    int added = 0;
    bool attempting = true;
    while (added < 42) {
        while (attempting) {
            int a = floor(14 * rand() / RAND_MAX);
            int b = floor(3 * rand() / RAND_MAX);
            if (revisions[a][b].periodID == Nothing) {
                revisions[a][b] = Period(toAdd[added], 8 + b, (Day) a);
                attempting = false;
            }
        }
        added++;
        attempting = true;
    }
    int writing = 0;
    Prediction* result = new Prediction;
    for (int day = 0; day < 14;day++) {
        for (int period = 0; period < 8; period++) {
            result->periods[writing] = Period(timetable[day][period],period,(Day) day);
            writing++;
        }
        for (int period = 8; period < 11;period++) {
            result->periods[writing] = revisions[day][period-8];
            writing++;
            
        }
    }
    return result;
}

Prediction* mutate(Prediction* pred) {
    Period res[154];
    copy(pred->periods, pred->periods + 154, res);
    int a_ind = floor(42 * (double)rand() / (double)RAND_MAX);
    a_ind = 11 * floor(a_ind / 3.0) + (a_ind % 3) + 8;
    int b_ind = floor(42 * (double)rand() / (double)RAND_MAX);
    b_ind = 11 * floor(b_ind / 3.0) + (b_ind % 3) + 8;
    res[a_ind] = pred->periods[b_ind];
    res[b_ind] = pred->periods[a_ind];
    Prediction* resPred = new Prediction(res);
    return resPred;
}
Prediction* fullMutation(Prediction* pred, int minScore, int maxScore,int thisScore) {
    double threshold = (thisScore - minScore) / (double)(maxScore - minScore);
    bool cont = true;
    int mutesLeft = 10;
    Prediction* res = mutate(pred);
    while (cont && mutesLeft > 0) {
        res = mutate(res);
        if (rand() / (double)RAND_MAX > threshold) {
            cont = false;
        }
        mutesLeft--;
    }
    return res;
}

int main()
{
    key = _getch();
    char test[2] = "h";
    cout << key;
    Prediction* initial[POPULATION];
    Prediction* temp[POPULATION];
    double total = 0;
    for (int j = 0;j < POPULATION;j++) {
        initial[j] = RandomPred();
        total += initial[j]->evaluate();
    }
    double best = 0;
    while (key != 'e') {
        qsort(initial, POPULATION, sizeof(Prediction*), [](const void* a, const void* b) {
            return (int)(round(100 *  (*(Prediction**)b)->evaluate()) - 100 * (*(Prediction**)a)->evaluate());
            });
        if (initial[0]->evaluate() > best) {
            best = initial[0]->evaluate();

            total = 0;
            for (Prediction* pred : initial) {
                total += pred->evaluate();
            }
            ofstream file;
            file.open("C:/Users/butch/OneDrive/Documents/timetableGeneticsResult.txt", std::ofstream::out | std::ofstream::trunc);
            file << initial[0]->format();
            file.close();
            cout << "\rBest score: " << initial[0]->evaluate() << "\tTotal Score: " << total;

        }
        
        if (key == 'a') {
        }
        for (int i = TOP_POPUL;i < MIDDLE_POPUL;i++) {
            initial[2 * i - TOP_POPUL] = fullMutation(
                initial[i],
                MIDDLE_POPUL,
                TOP_POPUL,
                i
            );

            initial[2 * i - TOP_POPUL+1] = fullMutation(
                initial[i],
                MIDDLE_POPUL,
                TOP_POPUL,
                i
            );
        }

    }
    
    

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
