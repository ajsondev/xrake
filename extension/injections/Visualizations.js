//Depends on Hide, Show, GetRect js files.

var XrakeCurrentGroup;
var XrakeCurrentGroupElements;
var XrakeMaxGroup = 0;
var XrakeInterval;

var XRAKE_MODE_RECT = 0;
var XRAKE_MODE_HIDE = 1;
var XRAKE_MODE_FADE = 2;
var XrakeCurrentPresentationMode = XRAKE_MODE_RECT;

function SetGroup(node, groupIndex) {
  if (groupIndex > XrakeMaxGroup)
    XrakeMaxGroup = groupIndex;
  node.setAttribute('data-xrake-group', groupIndex);
}

//private
function GetGroup(node) {
  return node.getAttribute("data-xrake-group");
  //document.querySelector("[data-xrake-group='"+uid+"']");
}

//private
function GetGroupNodes(groupIndex) {
  return document.querySelectorAll("[data-xrake-group='"+groupIndex+"']");
}

//private
function isInNodeList(list, node) {
  for(var i = 0; i < list.length; ++i)
    if (list[i] === node)
      return true;
  return false;
}

function forEachAncestor(node, func) {
  if (node == null || node == document.body)
    return;
  func(node.parentNode);
  forEachAncestor(node.parentNode, func);
}

var XrakeRects = [];

function AddRect(node) {
  var r = document.createElement("div");
  var rect  = GetRect(node);
  r.style.zIndex = 999999999;
  r.style.background = 'blue';
  r.style.opacity = 0.2;
  r.style.position = "absolute";
  r.style.left = rect[0] + "px";
  r.style.top = rect[1] + "px";
  r.style.width = rect[2] + "px";
  r.style.height = rect[3] + "px";
  r.setAttribute("matches-Xrake-Uid", uidFromNode(node));
  r.style.pointerEvents = "none";

  XrakeRects.push(document.body.appendChild(r));
}

function DeleteAllRects() {
  for(var i = 0; i < XrakeRects.length; ++i)
    document.body.removeChild(XrakeRects[i]);
  XrakeRects = [];
}

function ShowGroupFallback(groupIndex) {
  var group_nodes = GetGroupNodes(groupIndex);
  if (!group_nodes || group_nodes.length == 0)
    return;
  XrakeCurrentGroupElements = group_nodes.length;

  var hideFunc = Hide;
  var showFunc = Show;
    
  if (XrakeCurrentPresentationMode === XRAKE_MODE_FADE) {
    hideFunc = function(n) {n.style.opacity = 0.3};
    showFunc = function(n) {n.style.opacity = 1};
  }

  // Hide all.
  forEeachElement(document.body, function(node) {
    hideFunc(node);
  });

  for(var i = 0; i < group_nodes.length; ++i) {
    var node = group_nodes[i];
    forEachAncestor(node, showFunc);
    forEeachElement(node, showFunc);
  }
}

function ShowGroup(groupIndex) {
  if (XrakeCurrentPresentationMode !== XRAKE_MODE_RECT)
    return ShowGroupFallback(groupIndex);

  // XRAKE_MODE_RECT
  var group_nodes = GetGroupNodes(groupIndex);
  if (!group_nodes || group_nodes.length == 0)
    return;
  XrakeCurrentGroupElements = group_nodes.length;

  DeleteAllRects();

  for(var i = 0; i < group_nodes.length; ++i) {
    AddRect(group_nodes[i]);
  }
}

//private
// Reset means everithing is 'visible'
function ResetSelection() {
  var showFunc = Show;
  if (XrakeCurrentPresentationMode === XRAKE_MODE_FADE)
    showFunc = function(n) {n.style.opacity = 1};
  else if (XrakeCurrentPresentationMode === XRAKE_MODE_RECT)
    showFunc = DeleteAllRects;

  forEeachElement(document.body, function(node) {
    showFunc(node);
  });
}

//private
function NextGroup() {
  if (XrakeCurrentGroup == XrakeMaxGroup)
    XrakeCurrentGroup = 0;
  else
    ++XrakeCurrentGroup;
  ShowGroup(XrakeCurrentGroup);
}

//private
function PrevGroup() {
  if (XrakeCurrentGroup == 0)
    XrakeCurrentGroup = XrakeMaxGroup;
  else
    --XrakeCurrentGroup;
  ShowGroup(XrakeCurrentGroup);
}

//private
function AnimateSetTimer(ms) {
  clearInterval(XrakeInterval);
  XrakeInterval = setInterval(NextGroup, ms);
}

function StartAnimate() {
  ShowGroup(0);
  XrakeCurrentGroup = 0;
  //XrakeInterval = setInterval(ChangeGroup, 4000);
  OpenGroupControlsWindow();
}

function StopAnimate() {
  clearInterval(XrakeInterval);
}


//private
var XrakeGroupControlsWindow;
function OpenGroupControlsWindow() {
  XrakeGroupSwitcherWindow = window.open("", "", "width=300, height=250");
  XrakeGroupSwitcherWindow.document.write(XrakeGroupControlsWindowHTML);
  var next = XrakeGroupSwitcherWindow.document.getElementById("next");
  var prev = XrakeGroupSwitcherWindow.document.getElementById("prev");
  var gr = XrakeGroupSwitcherWindow.document.getElementById("currentGroup");
  var el = XrakeGroupSwitcherWindow.document.getElementById("currentElements");
  var mode = XrakeGroupSwitcherWindow.document.getElementById("mode");
  gr.innerText = XrakeCurrentGroup; el.innerText = XrakeCurrentGroupElements;
  next.onclick = function() { NextGroup(); gr.innerText = XrakeCurrentGroup; el.innerText = XrakeCurrentGroupElements; };
  prev.onclick = function() { PrevGroup(); gr.innerText = XrakeCurrentGroup; el.innerText = XrakeCurrentGroupElements; };
  mode.onclick = function() {
    ResetSelection();
    XrakeCurrentPresentationMode++;
    XrakeCurrentPresentationMode %= 2;
    ShowGroup(XrakeCurrentGroup);
  };
}

var XrakeGroupControlsWindowHTML = "<!doctype html> <html> <head> <style> body { font-size: 17px; } button { height: 30px; font-weight: bold; margin: 4px; width: 200px; } </style> </head> <body><center><button id=\"next\">Next</button><br/> <button id=\"prev\">Previous</button><br/> <p>Group: <strong id=\"currentGroup\"> 0 </strong> </p> <p>Elements: <strong id=\"currentElements\"> 0 </strong> </p>  <button id=\"mode\">Change mode</button> </body> </center> </html>";
