function GetChildren(node) {
  var children = [];
  if (node) {
    for (var i = 0; i < node.childElementCount; ++i) {
      var child = node.children[i];
      if (!ignoreNodeTraversal(child)) {
        var childUid = uidFromNode(child);
        children.push(childUid);
      }
    }
  }
  return {"children" : children};
}
