//
//  processingTask.cpp
//
//  Created by Farshad on 3/2/18.
//

#include <stdio.h>
#include "processingTask.h"
#include <algorithm>
#include <cmath>


ProcessResult calc_process_stats(Tweet &tweet) {
    ProcessResult result;
    result.size = tweet.subFrame[0].value;
    result.status=33;
    return result;
}
