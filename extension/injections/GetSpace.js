function GetSpace(node) {
  // values order: left top bottom right
  var s = window.getComputedStyle(node);
  var margins =  [ parseInt(s.marginLeft),
                   parseInt(s.marginTop),
                   parseInt(s.marginBottom),
                   parseInt(s.marginRight)
                 ];
  var paddings = [ parseInt(s.paddingLeft),
                   parseInt(s.paddingTop),
                   parseInt(s.paddingBottom),
                   parseInt(s.paddingRight)
                 ];
  return {"margins" : margins, "paddings" : paddings};
}
