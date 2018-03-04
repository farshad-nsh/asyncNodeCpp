/**
 * Created by macbook on 3/2/18.
 */
var fourierProcess = require("./cpp/build/Release/processingTask.node");
var tweet = {
    id : 3421, category : "111",
    subFrame : [
        { date : "2015-06-07", time :"12:30" ,value:"21" }, //subframe1
        { date : "2015-06-08", time :"14:30" ,value:"20" }, //subframe2
        { date : "2015-06-09", time :"17:30" ,value:"22" }, //subframe3
        { date : "2015-06-010", time :"18:30" ,value:"25" } //subframe4
    ]
};

// calling asynchronous c++ addon
fourierProcess.calculate_results_async(tweet,
    function(err, result) {
       console.log("typeof  result="+typeof  result);
        console.log("keys  of result="+Object.keys(result));
        if (err ) {
            console.log(err);
        }
        else {
            console.log("result.status="+result[0].status);
            console.log("result.size="+result[0].size);
            console.log("here!")
        }
    });

for(var i=0;i<3;i++)
{
    console.log("do something else at time "+i+" ...");
}



/*
var tweets=[];
tweets.push({
        id: 5555, category: "pumps", frame: [
            {date: "2015-06-07", time: "4:30", value: "41"},
            {date: "2015-06-08", time: "11:30", value: "40"},
            {date: "2015-06-09", time: "13:30", value: "42"},
            {date: "2015-06-010", time: "19:30", value: "45"}
        ]
    }
);
tweets.push({
        id: 5556, category: "pumps", frame: [
            {date: "2015-06-07", time: "4:31", value: "61"},
            {date: "2015-06-08", time: "11:31", value: "60"},
            {date: "2015-06-09", time: "13:31", value: "62"},
            {date: "2015-06-010", time: "19:31", value: "65"}
        ]
    }
)
*/
// utility printing

/*
results=
["normal","normal","normal","faulty"]
if tweet.Frame.value>22 ----> results[3].status="faulty"
 */

