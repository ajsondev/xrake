var xrakeNextUid = 1;
//var xrakeUidMappings = {};
//var xrakeReverseUidMappings = {};

function bindNextUid(node) {
  // Body can also be recreated dynamically.
  var uid = (node == document.body) ? 0 : xrakeNextUid++;
  node.setAttribute('data-xrake-uid', uid);
  return uid;
}

function nodeFromUid(uid) {
  return document.querySelector("[data-xrake-uid='"+uid+"']");
  //return xrakeUidToNodeMap[uid];
}

function uidFromNode(node) {
  var uid = node.getAttribute("data-xrake-uid");
  if (!uid)
    uid = bindNextUid(node);
  return parseInt(uid);
  //return xrakeNodeToUidMap[node];
}

function traverseDomForUids(node) {
  if (ignoreNodeTraversal(node))
    return;
  bindNextUid(node);
  for (var i = 0; i < node.childElementCount; ++i)
    traverseDomForUids(node.children[i]);
}

/**
 * Returns a random integer between min (inclusive) and max (inclusive)
 * Using Math.round() will give you a non-uniform distribution!
 * https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/random
 */
function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

//external
function GetRandomUid() {
  //exclude body!
  console.log("GRU");
  var uid = getRandomInt(1, xrakeNextUid-1);
  if(!nodeFromUid(uid))
    return -1; // element already deleted (unlikely but possible)
  return uid;
}

// Uid attribute is used as a pointer to node in the server code.
// Note: root element should have uid=0.
traverseDomForUids(document.body);