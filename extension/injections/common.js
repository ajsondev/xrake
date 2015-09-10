function busyWait(milliSeconds) {
  var startTime = new Date().getTime();
  while (new Date().getTime() < startTime + milliSeconds);
}

// Received function call arguments preprocessing.
function prepareArgs(args) {
  if (args && args.uid) {
    args.node = nodeFromUid(args.uid);
    if (!args.node)
      console.log("Unknown uid used as a function call argument!");
  }
  return args;
}

var XrakeHelperWindow;
function helperWindowAppend(node) {
  if(!XrakeHelperWindow || !XrakeHelperWindow.document)
    XrakeHelperWindow = window.open("", "", "width=400, height=600");
  XrakeHelperWindow.document.body.appendChild(node);
}

function ignoreNodeTraversal(node) {
  return (node.tagName === "SCRIPT" ||
          node.tagName === "STYLE"  ||
          node.tagName === "OBJECT" ||
          node.tagName === "BR" );
}

// applyFunction(node, depth)
function forEeachNode(root, applyFunction, depth) {
  if (!depth)
    depth = 1; // Body depth is 1.
  if (ignoreNodeTraversal(root))
    return;
  applyFunction(root, depth);
  for (var i = 0; i < root.childNodes.length; ++i)
        forEeachNode(root.childNodes[i], applyFunction, depth + 1);
}

// applyFunction(node) function isn't applied to TEXT and COMMENT nodes.
function forEeachElement(root, applyFunction) {
  if (ignoreNodeTraversal(root))
    return;
  applyFunction(root);
  for (var i = 0; i < root.childElementCount; ++i)
        forEeachElement(root.children[i], applyFunction);
}

// When applyFunction(node) returns true subtree processing ends.
function forEeachElementUntil(root, applyFunction) {
  if (ignoreNodeTraversal(root))
    return;
  if (applyFunction(root))
    return;
  for (var i = 0; i < root.childElementCount; ++i)
        forEeachElement(root.children[i], applyFunction);
}

// returns only directly embeded text (unlike node.innerText!)
function getNodeText(node) {
  var text = "";
  for (var i = 0; i < node.childNodes.length; ++i) {
    if (node.childNodes[i].nodeType === node.TEXT_NODE)
      text += node.childNodes[i].textContent.trim();
  }
  return text;
}

function matchNode(textsArray) {
  var textsFound = [];
  var minDepth = Infinity;
  forEeachNode(document.body, function(node, depth) {
    if (node.nodeType != node.TEXT_NODE)
      return;
    for (var i=0; i < textsArray.length; ++i) {
        if (!textsFound[i] && node.textContent.indexOf(textsArray[i]) != -1) {
          textsFound[i] = [node, depth];
          minDepth = Math.min(minDepth, depth);
        }
    }
  });

  // All texts found?
  if (textsFound.length != textsArray.length)
    return null;

  // Trim to equal chain length.
  for (var i = 0; i < textsFound.length; ++i) {
    var jumpBackward = textsFound[i][1] - minDepth;
    for (var j = 0; j < jumpBackward; ++j)
      textsFound[i][0] = textsFound[i][0].parentNode;
  }

  var commonAncestorFound = false;
  while (true) {
    commonAncestorFound = true;
    for (var i = 1; i < textsFound.length; ++i) {
      if (textsFound[i-1][0] != textsFound[i][0]) {
        commonAncestorFound = false;
        break;
      }
    }

    if (commonAncestorFound) {
      var resultNode = textsFound[0][0];
      if (resultNode.nodeType === resultNode.TEXT_NODE)
        resultNode = resultNode.parentNode;

      console.log("common.matchNode");
      console.log(resultNode);

      return resultNode;
    }

    // Step one node backward.
    for (var i = 0; i < textsFound.length; ++i) {
      textsFound[i][0] = textsFound[i][0].parentNode;
    }
  }

  // Not reached.
  return null;
}

// TODO: move to markers.js ?

function filteredNodesArray(nodeList) {
  var result = [];
  for(var i=0; i<nodeList.length; ++i)
    if(!ignoreNodeTraversal(nodeList[i]))
      result.push(nodeList[i]);
  return result;
}

function markerFromNode(node) {
  var json = {"chain" : [], "children": [], "type" : "list"};
  filteredNodesArray(node.children).forEach(function(child) {
    json["children"].push(child.tagName);
  });
  
  while(node != document) {
    var index = filteredNodesArray(node.parentNode.children).indexOf(node);
    json["chain"].push([node.tagName, index]);
    node = node.parentNode;
  }

  return json;
}

var markersCache = null;
function NodeFromMarker(marker_as_str) {
  if (!markersCache) {
    markersCache = {};
    forEeachElement(document.body, function(node) {
        var json_as_str = JSON.stringify(markerFromNode(node));
        if (markersCache[json_as_str])
          console.log("MARKER DUPLICATION !!!");
        markersCache[json_as_str] = node;
    });
  }

  var uid = -1;
  // reparse for compatibility
  marker_as_str = JSON.stringify(JSON.parse(marker_as_str));
  if (markersCache[marker_as_str])
    uid = uidFromNode(markersCache[marker_as_str]);

  return {"uid" : uid};
}


// scroll whole page
var PageExercised = false;
function ExercisePage() {
  var kStepPx = 600;
  var kStepTimeMs = 100;

  var scrolls = Math.ceil((document.body.scrollHeight - window.innerHeight) / kStepPx);
  var i1 = setInterval(function() {
    window.scrollBy(0, kStepPx);
  }, kStepTimeMs);
  setTimeout(function() {
    clearInterval(i1);
    window.scrollTo(0, 0);
    PageExercised = true;
  }, scrolls*kStepTimeMs);
}

//ExercisePage(); console.log("ExercisePage");
PageExercised = true;

function range_0_1(d) {
  return Math.min(Math.max(d, 0), 1);
}

