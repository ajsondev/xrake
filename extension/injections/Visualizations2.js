
function MarkNode(node) {
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

  document.body.appendChild(r);
}
