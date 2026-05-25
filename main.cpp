#include <cmath>
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <typeinfo>


using namespace std;

class Object {
public:
    string name = "ghost";
    string type = "ghost";
    double distance = 0;//in parsecs
    double apparentMagnitude = 0;
    double absoluteMagnitude = 0;
    int columnWidth = 20;

    double absolute_magnitude() {
        return apparentMagnitude + 5 - 5*log10(distance);
    }

    virtual void read_line(stringstream& ss) {//function that reads the lines from the txt file and uses it to populate the class
        string tempData;
        getline(ss, name, ',');
        getline(ss, type, ',');
        getline(ss, tempData, ',');
        distance = stod(tempData.c_str());
        getline(ss, tempData, ',');
        apparentMagnitude = stod(tempData);
    }

    void  virtual print_report() {
        ofstream fout;

        fout.open("Objects.txt",ios::app);
        fout << "|"<< setw(columnWidth) << name<< "|" << setw(columnWidth) << type << "|"<< setw(columnWidth) << distance<< "|" << setw(columnWidth) << apparentMagnitude<< "|"<< setw(columnWidth) << absolute_magnitude()<< "|";
    }

    void virtual print_header() {
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << left << "|" << setw(columnWidth) << "Name" << "|" << setw(columnWidth) << "Type"<< "|"<< setw(columnWidth) << "Dist (pc)"<< "|" << setw(columnWidth)<< "App Mag"<< "|"<< setw(columnWidth) << "Abs Mag"<< "|";
    };
    virtual string get_obj_class() = 0;//virtual func that allows us to print the object type
    string get_name() {return name;}
    double get_distance() {return distance;}
    double get_apparentMagnitude() {return apparentMagnitude;}
    ~Object(){}
};

class Star : public Object {
private:
    double solarMasses; //in solar masses
    double varPeriod; //variability period in Earth days

public:

    void read_line(stringstream& ss) override {
        Object::read_line(ss); //read common fields first
        string tempData;
        getline(ss, tempData, ',');
        solarMasses = stod(tempData.c_str());
        getline(ss, tempData, ',');
        varPeriod = stod(tempData.c_str());
    }

    void print_header() override {
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << endl << "Stars" << endl << endl;
        Object::print_header();
        fout << setw(columnWidth) << "Mass (Solar Masses)" << "|"  << setw(columnWidth) << "Var Period (days)" << "|" << endl;
}

    void print_report() override {
        Object::print_report();
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << setw(columnWidth) << solarMasses <<"|";
        if (varPeriod==0){ fout << setw(columnWidth) << "none" << "|" << endl;}else{fout << setw(columnWidth) << varPeriod <<"|"<< endl;}
    }
    string get_obj_class() {return "star";}
    ~Star(){}
};

class Planet : public Object {
private:
    double earthMasses;
    string parentStar;
    double orbitalPeriod; //in Earth years

public:

    void read_line(stringstream& ss) override {
        Object::read_line(ss);
        string tempData;
        getline(ss, tempData, ',');
        earthMasses = stod(tempData.c_str());
        getline(ss, tempData, ',');
        parentStar = tempData.c_str();
        getline(ss, tempData, ',');
        orbitalPeriod = stod(tempData.c_str());
    }

    void print_header() {
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << endl << "Planets" << endl << endl;
        Object::print_header();
        //fout << left << "|" << setw(columnWidth) << "Name" << "|" << setw(columnWidth) << "Type"<< "|"<< setw(columnWidth) << "Dist (pc)"<< "|" << setw(columnWidth)<< "App Mag"<< "|"<< setw(columnWidth) << "Abs Mag"<< "|" << setw(columnWidth) << "Mass (Earth Masses)" << "|"<< endl;
        fout << left << setw(columnWidth) << "Mass (Earth Masses)" << "|"<< setw(columnWidth)<< "Parent Star" <<"|"<< setw(columnWidth)<< "Orbital Period (yrs)" << "|" <<endl;
    }

    void print_report() override {
        Object::print_report();
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << setw(columnWidth) << earthMasses <<"|"<< setw(columnWidth) << parentStar <<"|"<< setw(columnWidth)<< orbitalPeriod<< endl;
    }

    string get_obj_class() {return "planet";}
    ~Planet(){}
};

class Galaxy : public Object {
    private:
    double redShift; // how many nanometers of redshift
    double numStars;//measured in billions of stars - hence why it is a double rather than int
    double recessionVelocity; //calculated from
    double agreementWithHubble; //percentage deviation from that expected by Hubble's constant
    double const hubbleConst = 70; //Hubbles constant in km/s/Mpc

    public:
    void read_line(stringstream& ss) override {
        Object::read_line(ss);
        string tempData;
        getline(ss, tempData, ',');
        redShift = stod(tempData.c_str());
        getline(ss, tempData, ',');
        numStars = stod(tempData.c_str());
        getline(ss, tempData, ',');
        recessionVelocity = stod(tempData.c_str());
    }

    void print_header(){
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << endl << "Galaxies" << endl << endl;
        Object::print_header();
        fout << left << setw(columnWidth) << "Redshift (nm)" << "|" << setw(columnWidth) << "Num Stars (billions)" << "|" << setw(columnWidth) << "R Velocity (km/s)" << "|"<< setw(columnWidth) << "H0 Disagreement (%)" << "|" <<endl;
    }

    void print_report() override {
        Object::print_report();
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << setw(columnWidth) << redShift <<"|" << setw(columnWidth) << numStars << "|"  << setw(columnWidth) << recessionVelocity <<"|" << setw(columnWidth) << agreement_with_hubble() << "|" << endl;
    }

    double agreement_with_hubble() {
        agreementWithHubble = 100*(recessionVelocity-(hubbleConst*distance/pow(10,6)))/(hubbleConst*distance/pow(10,6)) ;
        return agreementWithHubble;
    }

    double get_redShift() {return redShift;}
    double get_numStars() {return numStars;}
    string get_obj_class() {return "galaxy";}
    ~Galaxy(){}
};

class Nebula : public Object {
private:
    double density; //in 10,000 atoms /cm^3
    double width; //In light years

public:

    void read_line(stringstream& ss) override {
        Object::read_line(ss); //read common fields first
        string tempData;
        getline(ss, tempData, ',');
        density = stod(tempData.c_str());
        getline(ss, tempData, ',');
        width = stod(tempData.c_str());
    }

    void print_header() override {
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << endl << "Nebulae" << endl << endl;
        Object::print_header();
        fout << setw(columnWidth) << "Density 10^5atom/cm3" << "|"  << setw(columnWidth) << "Width (lyrs)" << "|" << endl;
    }

    void print_report() override {
        Object::print_report();
        ofstream fout;
        fout.open("Objects.txt",ios::app);
        fout << setw(columnWidth) << density << "|" << setw(columnWidth) << width << "|"<<endl;
    }
    string get_obj_class() {return "nebula";}
    ~Nebula(){}
};

enum Type{star, planet, galaxy, nebula};//list of the possible instances of type, these correpsond to the derived/real classes we are using

class ObjectFactory {//function used to read in the first entry for each line and produce an object of that class
    public:
    Object* create(Type t) {
        switch(t) {
            case star:
                return new Star();
            case planet:
                return new Planet();
            case galaxy:
                return new Galaxy();
            case nebula:
                return new Nebula();
        }
    }
};

int main() {
    string fileName = "AstroObjects.txt";
    string objClass;
    vector<Object *> objects;
    ObjectFactory factory;
    char repeat = 'y';
    //int columnWidth = 15;

    map<string, Type> inputMap = {
        {"star", star},
        {"planet", planet},
        {"galaxy", galaxy},
        {"nebula", nebula}
    };

    ifstream file(fileName);
    stringstream ss;

    if(file.fail()) {
        cout<<"Error opening file"<<endl;
        return 1;//Signals to shell something has gone wrong with the program
    }

    if (file.is_open()) {
        string line;
        while(getline(file,line)) { //Each line is extracted as a string
            stringstream ss(line);
            string data;

            getline(ss, data, ',');

            if (inputMap.find(data) == inputMap.end()) {
                cerr << "unknown type: " << data << endl;
            }

            Object* object = factory.create(inputMap[data]);
            object->read_line(ss);
            objects.push_back(object);
        }
    }

    while (repeat == 'y') {
        string dataLine;
        string data;
        cout << "Would you like to add any more objects? If so please enter in the format given by the user guide, if not, please press any character, then enter" << endl;
        cin >> dataLine;
        stringstream ss(dataLine);

        getline(ss, data, ',');

        if (inputMap.find(data) == inputMap.end()) {
            break; //break statement used to leave the loop if the user doesn't want to input another entry
        }

        Object* object = factory.create(inputMap[data]);
        object->read_line(ss);
        objects.push_back(object);

        cout << "would you like to make another entry? Enter 'y' for yes or any other character for no" << endl;
        cin >> repeat;
    }

    ofstream fout;
    fout.open("Objects.txt");
    for (int i=0; i<50; i++) fout << "="; fout <<endl;
    fout << "Catalogue of Astronomical Objects" << endl;//Creates the header in the text file and also overwrites existing file
    for (int i=0; i<50; i++) fout << "="; fout << endl;

    cout << "Which object categories would you like in the report? Enter 'all' or specify an object category, in lower case" << endl;//validation for input when selecting what is to be printed
    while (!(cin >> objClass) || objClass != "all" && objClass != "star" && objClass != "planet" && objClass != "galaxy" && objClass != "nebula"  ) {
        cout << "Invalid input, must be 'all', 'star', 'planet', 'galaxy' or 'nebula'"<<endl;
        cin.clear();
        cin.ignore();
    }

    if (objClass == "all") {//If all objects are selected they are ordered then printed
        vector<string> objClasses = {"star", "planet", "galaxy", "nebula"};//creates array of the possible classes so they can be cycled through

        for (int j=0; j<objClasses.size(); j++) {
            int i = 0;
            for (Object*& o : objects) {
                if (o->get_obj_class() == objClasses[j]) {//prints reports for the chosen classes
                    if (i == 0) {o->print_header(); i++;}//prints the header, but will only run once per loop as i will no longer equal 0 after this line runs
                    o->print_report();
                }
            }
        }
    }
    else {//if only one object is chosen then that one is printed
    int i = 0;
        for (Object*& o : objects) {
            if (o->get_obj_class() == objClass) {//prints reports for the chosen classes
                if (i == 0) {o->print_header(); i++;}//prints the header, but will only run once per loop as i will no longer equal 0 after this line runs
                o->print_report();
            }
        }
    }

    //cleanup
    for (Object*& o : objects) {
        delete o;
    }
    objects.clear();

    cout<<"Report finished"<<endl;

    return 0;
}
