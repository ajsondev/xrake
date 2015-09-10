function AllElements() {
  var nodes = [];
  forEeachElement(document.body, function(node) {
    var uid = uidFromNode(node);
    if (uid > 0)
      nodes.push(uid);
  });
  return {"nodes" : nodes};
}
