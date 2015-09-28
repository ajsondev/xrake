function markNodeInternal(node, color, opacity, attr_suf) {
  if(!node)
    return; //???
  var r = document.createElement("div");
  var rect  = GetRect(node);
  r.style.zIndex = 999999999;
  r.style.background = color;
  r.style.opacity = opacity;
  r.style.position = "absolute";
  r.style.left = rect[0] + "px";
  r.style.top = rect[1] + "px";
  r.style.width = rect[2] + "px";
  r.style.height = rect[3] + "px";
  r.setAttribute("matches-Xrake-Uid"+attr_suf, uidFromNode(node));
  r.style.pointerEvents = "none";

  document.body.appendChild(r);
}

var markedNodeCount = 0; 
var marekNodes = [];
var marekNodesList = [];

function MarkNode(node) {
  if(!node)
    return;

  if(uidFromNode(node) == 3) return;

  setTimeout(function() {markNodeInternal(node, 'blue', 0.2, '')},
    markedNodeCount*0);
  
  markedNodeCount++;
  marekNodes[uidFromNode(node)] = true;
  marekNodesList.push(node);
}

function MarkCommonRoots(node) {

  var cssText = "div[matches-Xrake-Uid] { display: none; }";
  var css = document.createElement("style");
  css.type = "text/css";
  if("textContent" in css)
      css.textContent = cssText;
  else
      css.innerText = cssText;
  document.body.appendChild(css);

debugger;

  for(var i=0; i<marekNodesList.length; ++i) {
    var node = marekNodesList[i];
    var origNode = node;
    var asc_marked = false;
    node = node.parentNode;
    while(node && node != document) {
      if (marekNodes[uidFromNode(node)]) {
        asc_marked = true;
        break;
      }
      node = node.parentNode;
    }
    if (!asc_marked) {
      markNodeInternal(origNode, 'green', 0.2, '-green');
    }
  }

}
