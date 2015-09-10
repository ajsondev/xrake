function GetParent(node) {
  var parentUid = -1; // Means no parent or node not found.
  // It simulates that body tag node is root and has no parent.
  if (node && uidFromNode(node) > 0) {
    node = node.parentNode;
    parentUid = uidFromNode(node);
  }
  return {"parent" : parentUid};
}
