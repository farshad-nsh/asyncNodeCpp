//
//  processingTask.h
//
//  Created by Farshad on 3/2/18.
//
//

#ifndef processingTask_h
#define processingTask_h


#endif /* processingTask_h */

#include <vector>
#include <string>
using namespace std;

class SubFrame {
public:
    SubFrame() {
        double id =32; double category = 12;
    }
    SubFrame (string date, double time,double value) {
        date = date;
        time = time;
        value=value;
    }
    string date;
    string time;
    double value;
};


class Tweet {
public:
    string category;
    string id;
    vector<SubFrame> subFrame;
};


class ProcessResult {
public:
    double size;
    double status;
};

ProcessResult calc_process_stats(Tweet &tweet);

