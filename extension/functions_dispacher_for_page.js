// Set contains ids of pages which was opened by this extension and not closed yet.
var ownedPages = {};
// TODO: remove page also after navigation ?

function dispatchFunctionForPage(pageId, funName, args, sendResult) {
  if (!ownedPages[pageId]) {
    // Page doesn't exsists or we dont't own it.
    console.log("Fatal: action requested for unknown page!");
    return;
  }

  var runnerCode = funName;
  if (funName == "AllElements"  ||
      funName == "HideAll"      ||
      funName == "StartAnimate" ||
      funName == "StopAnimate"  ||
      funName == "GetFeaturesJumbo" ||
      funName == "GetRandomUid")
    runnerCode += "()";
  else if (funName == "GetChildren" ||
           funName == "GetParent"   ||

           funName == "GetSpace"    ||
           //funName == "GetNormalizedSpace" ||
           funName == "Hide"        ||
           funName == "NextElement" ||

           funName == "GetHistogramTextColorGrayscale" ||
           funName == "GetHistogramTextSize" ||
           funName == "GetWordCountPerChild" ||
           funName == "GetChildrenRectVariance" ||
           funName == "PqGramDistanceChildrenMean" ||
           funName == "GetNormalizedRect"  ||
           funName == "GetRect"     ||

           funName == "Show" ||
           funName == "MarkNode")
    runnerCode += "(XrakeArgs.node)";
  else if (funName == "MatchNode" ||
           funName == "NodeFromMarker")
    runnerCode += "(XrakeArgs.str)";
  else if (funName == "GetHistogramTextColor")
    runnerCode += "(XrakeArgs.node, XrakeArgs.colorsCount)";
  else if (funName == "SetGroup")
    runnerCode += "(XrakeArgs.node, XrakeArgs.groupIndex)";
  else if (funName == "GetTrainingJson")
    return GetTrainingJsonAsync(pageId, sendResult);

  if (runnerCode == funName) {
    // Function not recognized.
    console.log("Fatal: unknown RPC function requested!");
    return;
  }

  // Warning: possibly dangerous args can be injected TODO: consider
  var argsCode = "var XrakeArgs = prepareArgs(" + JSON.stringify(args) + "); ";
  chrome.tabs.executeScript(pageId, { "code" : argsCode + runnerCode }, function(resArray) {
    sendResult(resArray[0])
  });
}


var pendingGetTrainingJson;
function GetTrainingJsonAsync(pageId, sendResult) {
  pendingGetTrainingJson = sendResult;
  // Hook listener for finalization. TODO: consider multiple calls
  chrome.runtime.onMessage.addListener(
    function(request, sender, sendResponse) {
      if (pendingGetTrainingJson && request.finalizeGetTrainingJson) {
        pendingGetTrainingJson(request.finalizeGetTrainingJson);
        pendingGetTrainingJson = null;
      }
    }
  );

  chrome.tabs.executeScript(pageId, { "file" : "injections/GetTrainingJson.js" });
}

function SkipGetTrainingJson() {
  if (pendingGetTrainingJson) {
    pendingGetTrainingJson({"skip": true});
    pendingGetTrainingJson = null;
  }
}