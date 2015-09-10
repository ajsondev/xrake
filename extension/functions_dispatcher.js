function dispatchFunction(json, sendResult) {
  var funName = json.fun;
  var args = json.xargs;
  var pageId = json.forwardTo;
  if (funName == "TestFunction")
    sendResult(GlobalTestFunction(args));
  else if (funName == "OpenPage")
    GlobalOpenPage(args, sendResult);
  else if (funName == "ClosePage")
    sendResult(GlobalClosePage(args));
  else if (funName == "GetWarcLoadedPages")
    GetWarcLoadedPages(sendResult);
  else if (pageId) {
    // Delay until page is fully loaded.
    chrome.tabs.executeScript(pageId, {"code" : "PageExercised && document.readyState == 'complete'"}, function(resArray) {
        if (resArray[0]) {
          dispatchFunctionForPage(pageId, funName, args, sendResult);
        } else {
          setTimeout(function() {dispatchFunction(json, sendResult)}, 500);
          console.log(funName, "is waiting...");
        }
    });
  }
}


// ---- Global functions -----
function GlobalTestFunction(args) {
  console.log(args);
  return {"res2" : "RES" + args.arg2 + args.arg2, "res1": 4 * args.arg1};
}


function GlobalOpenPage(args, sendResult) {
  chrome.tabs.create({"url" : args.url, "selected" : true}, function(tab) {
      InjectAllFunctions(tab.id, function() {
        ownedPages[tab.id] = true;
        sendResult({"pageId" : tab.id});
      });
  });
}

function GlobalClosePage(args) {
  if (args.pageId != -1) {
    try {
      delete ownedPages[args.pageId];
      chrome.tabs.remove(args.pageId);
    } catch(e) {}// To not block on closing already closed
  }
  return {};
}

var pendingWarcCallback = null;
var waitingWarcResults = null;

function GetWarcLoadedPages(sendResult) {
  if (pendingWarcCallback) {
    // Abort waiting call (if any).
    pendingWarcCallback({"urls": []});
  }
  
  pendingWarcCallback = sendResult;
}

// Hook listener for training_warc_discovery messages
chrome.runtime.onMessage.addListener(
  function(request, sender, sendResponse) {
    if (request.finalizeGetWarcLoadedPages && pendingWarcCallback) {
      pendingWarcCallback({"urls": request.finalizeGetWarcLoadedPages});
      pendingWarcCallback = null;
      chrome.tabs.remove(sender.tab.id);
    } 
    // else if (request.injectAllFunctionsToTrainingPage) {
    //   // Used for more bullet-proof injection (e.g. accidental page refresh)
    //   InjectAllFunctions(sender.tab.id, function() {
    //     chrome.tabs.executeScript(sender.tab.id, { "file" : "injections/GetTrainingJson.js" });
    //   });
    // }
  });
// --------------------------

// Prepare page to receive and execute remote functions.
function InjectAllFunctions(pageId, callback) {
  var files = [
    "injections/jqgram_jumbo.js",
    "injections/common.js",
    "injections/attachUids.js",
    "injections/AllElements.js",
    "injections/GetChildren.js",
    "injections/GetHistogramTextColor.js",
    "injections/GetHistogramTextSize.js",
    "injections/GetParent.js",
    "injections/GetRect.js",
    "injections/GetSpace.js",
    "injections/Hide.js",
    "injections/HideAll.js",
    "injections/MatchNode.js",
    "injections/NextElement.js",
    "injections/Show.js",
    "injections/Visualizations.js",
    "injections/Visualizations2.js",
    "injections/PqGramDistance.js",
    "injections/FeaturesJumbo.js",
  ]; // Files are added in order.
  InjectionsCascade(pageId, files, 0, callback);
}

function InjectionsCascade(pageId, files, index, callback) {
  if (index < files.length) {
    chrome.tabs.executeScript(pageId, {"file" : files[index]}, function() {
        InjectionsCascade(pageId, files, index + 1, callback);
    });
  } else if (callback) {
    callback();
  }
}
