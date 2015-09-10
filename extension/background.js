var SERVER_ADDRESS = "ws://localhost:5555/";
var MAX_FRAME_SIZE = 102400;
var ws = null;

var bytesSent = 0;
var bytesReceived = 0;

var DEBUG = false;

function printQuota() {
	console.log("bytes received: " + bytesReceived);
	console.log("bytes sent: " + bytesSent);
}

// Message: function call from server
function onMessage(msg) {
	var data = msg.data;
	if (DEBUG) {
		bytesReceived += data.length;
		console.log("-> "+data);
	}
	var json = JSON.parse(data);
	if (json.fun) {
		dispatchFunction(json, function(result) {ws.sendFragmented(result);});
	}
	//chrome.runtime.sendMessage({text: data});
	//ws.send("REPLY "+data);
}

function wsSendFragmented(ws, json) {
	var data = JSON.stringify(json);
	if (DEBUG) {
		bytesSent += data.length;
		console.log("<- "+data);
	}
	var start = 0;
	ws.send(data.length.toString());
	while (start < data.length) {
		var length = Math.min(data.length-start, MAX_FRAME_SIZE);
		ws.send(data.substr(start, length));
		//console.log("SF ", start, length);
		start += length;
	}
}

function ensureConnection(callback) {
	if (ws === null) {
		ws = new WebSocket(SERVER_ADDRESS);
		ws.onmessage = onMessage;
		ws.onerror = ws.onclose = function() {ws = null};
		ws.onopen = callback;
		ws.sendFragmented = function(json) {wsSendFragmented(ws, json)};
	} else {
		callback();
	}
}

function TestClassificationOnPage(use_bayes) {
	chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
		var tabId = tabs[0].id;
		InjectAllFunctions(tabId, function() {
			ownedPages[tabId] = true;
			ensureConnection(function() {
				ws.send("TestClassificationOnPage"+(use_bayes ? "Bayes" : "SVM")+" "+tabId);
			});
		});
	});
}


//document.addEventListener('DOMContentLoaded', function () {
// Message: action from popup
chrome.runtime.onMessage.addListener(
  function(request, sender) {
	if ('action' in request) {

	  // Local actions.
	  if(request.action == "TestClassificationOnPageBayes") {
	  	TestClassificationOnPage(true);
	  	return;
	  } else if(request.action == "TestClassificationOnPageSVM") {
	  	TestClassificationOnPage(false);
	  	return;
	  } else if(request.action == "SkipGetTrainingJson") {
	  	SkipGetTrainingJson();
	  	return;
	  } else if(request.action == "InjectTrainingCode") {
	  	chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
			InjectAllFunctions(tabs[0].id, function() {
				chrome.tabs.executeScript(tabs[0].id, { "file" : "injections/GetTrainingJson.js" });
			});
	  	});
	  	return;
	  }

	  // Non-local actions.
	  ensureConnection(function() {
			ws.send(request.action);
	  });
	 }
  });
//});
