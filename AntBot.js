"use strict";

const fs = require('fs');
const execSync = require('child_process').execSync;

var segmentsList = ['ALPHA','BETA','Q_PHENO','RELEASE_BEFORE',/*'RELEASE_AFTER',*/'START_PHENO','START_MUL','Q_DIST'];
var segmentsRange = {
	ALPHA : {min:0.5,max:2,step:0.2},
	BETA : {min:0.5,max:10,step:0.5},
	Q_PHENO : {min:1000,max:10000,step:1000},
	ANT_AMOUNT : {min:10,max:10,step:10},
	RELEASE_BEFORE : {min:0.5,max:1,step:0.05},
//	RELEASE_AFTER : {min:0.5,max:1,step:0.05},
	START_PHENO : {min:100,max:100,step:100},
	START_MUL : {min:10,max:10,step:1},
	Q_DIST : {min:1,max:1,step:1}
};
var number = 0;
function dfs(segments,segmentsNum) {
	if (segmentsNum == segmentsList.length) {
		console.log(segments,' ',segmentsNum);
		number ++;
		var outputString = "";
		var segString = "";
		for (var seg in segments) {
			outputString += /*seg + " " + */segments[seg] + "\n";
			segString += seg + " " + segments[seg] + "\n";
		}
		fs.writeFileSync('seg', outputString);
		var sum=0;
		for (var i=0;i<10;i++) {
			try {
				var child = execSync('./bin/future_net case2/topo.csv case2/demand.csv result',{timeout:3100});
			} catch (err) {
				console.log('force kill ant');
			}
			var cost = fs.readFileSync('results');
			//console.log(cost.toString());
			if (parseInt(cost.toString()) == 99999) {
				sum = 0;
				break;
			}
			sum += parseInt(cost.toString());
		}
		if (sum !=0) {
			fs.appendFileSync('Answers',segString + sum + "\n");
			console.log(sum);
		}
	} else {
		for (var i=segmentsRange[segmentsList[segmentsNum]].min; i<=segmentsRange[segmentsList[segmentsNum]].max; i=i+segmentsRange[segmentsList[segmentsNum]].step) {
			segments[segmentsList[segmentsNum]] = i;
			dfs(segments,segmentsNum+1);
		}
	}
}

var segments={};

dfs(segments,0)
