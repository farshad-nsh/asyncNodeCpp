//
//  processingTask_test.cpp
//  process
//
//  Created by Farshad on 3/2/18.



#include <stdio.h>
#include <iostream>
#include "processingTask.h"
using namespace std;

int main() {
    Tweet tweet;
    
    tweet.category = 111;
    tweet.id = 12;
    //we have one frame and for subFrame. you divide a signal into 4 samples
    tweet.subFrame.push_back(Subframe("2014-11-3", "12:30",21));
    tweet.subFrame.push_back(Subframe("2014-11-4", "12:31",22));
    tweet.subFrame.push_back(Subframe("2014-11-5", "12:32",23));
    tweet.subFrame.push_back(Subframe("2014-11-6", "12:33",24));

    
    cout << "Fourier transform or anything = " << calc_process_stats(tweet) << endl;
}