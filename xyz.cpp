#include <vector>
#include <string>


class Command {
public:
    bool intermediatelatchesactive;
    int intermediatelatchlength;
    int numberofstages;
    std::vector<int> stagelengths;
    int bypassindex;
    int readindex;
    int destinationregister;
    int sourceregister1;
    int sourceregister2;
    std::vector<std::string> stagenames;
    bool isbranch;
    int jumpindex;

    Command(bool in1, std::vector<int> in2, std::vector<int> in3, int in4, int in5, int in6, int in7, std::vector<int> in8) {
        intermediatelatchesactive = in1;
        intermediatelatchlength = in6;
        numberofstages = in2.size();
        stagelengths = in2;
        bypassindex = in4;
        readindex = in5;
        destinationregister = in3[0];
        sourceregister1 = in3[1];
        sourceregister2 = in3[2];
        stagenames = std::vector<std::string>(in7);
        isbranch = in8[0];
        jumpindex = in8[1];
    }
};

class Registerfile {
public:
    int size;
    std::vector<int> updatetime;
    std::vector<int> intermediateupdatetime;

    Registerfile(int in1) {
        size = in1;
        updatetime = std::vector<int>(size, 0);
        intermediateupdatetime = std::vector<int>(size, 0);
    }
};
class Runtimedata {
public:
    Command command;
    int starttime;
    std::vector<std::vector<int> > stages;

    Runtimedata(Command in1, int in2, int in3)
        : command(in1), starttime(in2), stages(in3, std::vector<int>(3, 0)) {}
};


class Pipeline {
public:
    int numberofstages;
    bool bypassactive;
    bool symmetryactive;
    std::vector<int> stageemptytime;
    std::vector<int> pseudostageemptytime;
    int numberofregisters;
    Registerfile registerfile=Registerfile(numberofregisters);
    Registerfile pseudoregisterfile=Registerfile(numberofregisters);
    std::vector<Runtimedata> pseudoruntimelist;
    std::vector<Runtimedata> history;

    Pipeline(int in1, bool in2, bool in3, int in4) {
        // numberofstages = in1;
        bypassactive = in2;
        symmetryactive = in3;
        stageemptytime = std::vector<int>(in1, 0);
        pseudostageemptytime = std::vector<int>(in1, 0);
        numberofregisters = in4;
    }

    void run(Command in1) {
        Runtimedata runtime_data(in1, 0, numberofstages);
        pseudoruntimelist.push_back(runtime_data);
        Command command = Command(false,in1.stagelengths,std::vector<int> {in1.destinationregister,in1.sourceregister1,in1.sourceregister2}, in1.bypassindex, in1.readindex, in1.intermediatelatchlength, in1.stagenames, std::vector<int> (2,0)); 
        
    }
};

    std::vector<int> my_vector = {1, 2, 3, 4, 5};

