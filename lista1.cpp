#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <random>
using namespace std;
typedef mt19937 rng_type;

rng_type rng;
//struct
struct city {
    double x;//coordinates
    double y;//coordinates
    double *distance; //table of distances to any other city
    bool tabu; //if the element is in tabu
    int time;//time an element stays in tabu
    bool visited; //for the first path
};

//Utils
//print path
void printGraph(int *result, vector<struct city> cities, int size) {
    for (int i = 0; i < size-1; i++) {
        cerr<< result[i]+1 << " ---> " << result[i + 1]+1 << " dist: " << cities.at(
                (unsigned long long int) result[i]).distance[result[i + 1]]
             << endl;
    }
    cerr << result[size-1] +1<< " ---> " << result[0]+1 << " dist: " << cities.at((unsigned long long int) result[size - 1]).distance[result[0]]
         << endl;
}
//sum of distances in the result
long double sumOfDistances(int *result, vector<struct city> &cities, int size) {
    long double sum = 0;
    for (int i = 0; i < size - 1; i++) {
        sum += cities.at((unsigned long long int) result[i]).distance[result[i + 1]];
    }
    sum+=cities.at((unsigned long long int) result[size - 1]).distance[result[0]];
    return sum;
}

//Counting distance
double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
}

//function for first path
int greedyDistance(struct city city, int size, vector<struct city> &cities) {
    int min = -1;
    unsigned i = 0;
    double minValue = -1;
    for (; i < size; i++) {
        if (city.distance[i] != 0) {
            if (!cities.at(i).visited) {
                minValue = city.distance[i];
                min = i;
                break;
            }
        }
    }
    if (minValue == -1) {
        return -1;
    }
    for (; i < size; ++i) {
        if (city.distance[i] < minValue && city.distance[i] != 0) {
            if (!cities.at(i).visited) {
                minValue = city.distance[i];
                min = i;
            }
        }
    }
    return min;
}
//function allowing to change 2 nodes in the graph
void opt2(int *result, int *result2, int size, int x, int y) {
    int i, j = 0;
    if(result[x]!=result[y]) {
        for (i = 0; i < x; i++) {
            result2[i] = result[i];
        }
        for (i = x; i < y + 1; i++) {
            result2[i] = result[y - j];
            j++;
        }
        for (i = y + 1; i < size; i++) {
            result2[i] = result[i];
        }
    }
}

int main() {
    int a, cityName, h = 0, w = 0;
    long double sum, radius;
    double xCity, yCity;
    long double pathSum = 0;
    string input;
    vector<struct city> cities;
    cin >> a;
    int *result = new int[a];
    for (int i = 0; i < a; i++) {
        struct city city;
        cin >> cityName;
        cin >> xCity >> yCity;
        city.x = xCity;
        city.y = yCity;
        city.tabu = false;
        city.time = 20;
        city.visited = false;
        cities.push_back(city);
    }
    for (unsigned i = 0; i < a; i++) {
        cities.at(i).distance = new double[a];
        for (int j = 0; j < a; j++) {
            cities.at(i).distance[j] = distance(cities[i].x, cities[i].y, cities[j].x, cities[j].y);
        }
    }
    result[0]=0;
    w=1;
    while (true) {
        cities.at((unsigned long long int) h).visited = true;
        h = greedyDistance(cities.at((unsigned long long int) h), a, cities);
        if (h == -1) {
            pathSum += cities.at((unsigned long long int) result[w - 1]).distance[0];
            w++;
            break;
        } else {
            result[w] = h;
            pathSum += cities.at((unsigned long long int) result[w - 1]).distance[result[w]];
            w++;
        }

    }
    rng_type::result_type const seedval = (const unsigned int) time(NULL);
    rng.seed(seedval);
    int *result2 = new int[a];
    int numOfIterations;
    if(a<3000){
        radius=pathSum;
        numOfIterations=1500;
    }else if(a<6000){
        radius=pathSum/a*10;
        numOfIterations=1500;
    }else if(a<10000){
        radius=pathSum/a*6;
        numOfIterations=1200;
    } else{
        radius=pathSum/a*2;
        numOfIterations=1000;
    }

    int iterations=a+numOfIterations;
    uniform_int_distribution<rng_type::result_type> udist(0, (unsigned int) (a - 1));
    while (iterations>0) {
        rng_type::result_type random_number = udist(rng);
        int random = random_number;
        if (!cities.at((unsigned long long int) result[random]).tabu) {
            cities.at((unsigned long long int) result[random]).tabu = true;
            cities.at((unsigned long long int) result[random]).time = 25;
            for (int i = 0; i < a; ++i) {
                if (result[random] != result[i] && cities.at(
                        (unsigned long long int) result[random]).distance[result[i]] < radius) {
                    opt2(result, result2, a, result[random], result[i]);
                    sum = sumOfDistances(result2, cities, a);
                    if (sum < pathSum) {
                        for (int l = 0; l < w; ++l) {
                            result[l] = result2[l];
                        }
                        pathSum = sum;
                        iterations=numOfIterations;
                    }
                }
            }
        }
        for (int i = 0; i < a; i++) {
            if (cities.at((unsigned long long int) i).tabu) {
                cities.at((unsigned long long int) i).time--;
                if (cities.at((unsigned long long int) i).time == 0) {
                    cities.at((unsigned long long int) i).tabu = false;
                }
            }
        }
        iterations--;
    }

    delete[] result2;
    cout<<pathSum<<endl;
    printGraph(result, cities, a);
    for (unsigned i = 0; i < a; i++) {
        delete[] cities.at(i).distance;
    }
    return 0;
}
