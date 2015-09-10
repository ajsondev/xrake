
function GetFeaturesJumbo() {
  var sample_set = [];
  forEeachElement(document.body, function(node) {
    if (node.childElementCount > 1) {
      sample_set.push({
        "uid": uidFromNode(node),
        "pq": PqGramDistanceChildrenMean(node),
        "wpc": GetWordCountPerChild(node),
        "crv": GetChildrenRectVariance(node)
        //"r": GetRect(node),
        //"rn": GetNormalizedRect(node),
        //"tc": GetHistogramTextColorGrayscale(node),
        //"ts": GetHistogramTextSize(node)
      });
    }
  });

  return sample_set;
}