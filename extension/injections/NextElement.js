function NextElement(node) {
    //todo: Not working!!!!

  if (!node) {
    console.log("NextNode: This should not happen!");
    return {"n" : -1};
  }
  var nNode;

  // First not ignored child.
  if (node.childElementCount > 0) {
    nNode = node.children[0];
    while(nNode && ignoreNodeTraversal(nNode))
      nNode = nNode.nextSibling;
    if (nNode)
      return {"n" : uidFromNode(nNode)};
  }

  // Next not ignored sibling.
  nNode = node.nextSibling;
  while(nNode && ignoreNodeTraversal(nNode))
    nNode = nNode.nextSibling;
  if (nNode)
    return {"n" : uidFromNode(nNode)};


  if(node != document.body &&
            node.parentNode != document.body) {
    nNode = node.parentNode;
  } else {
    // End of nodes.
    return {"n" : -1};
  }
  return {"n" : uidFromNode(nNode)};
}
