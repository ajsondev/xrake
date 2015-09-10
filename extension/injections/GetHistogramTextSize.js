
function GetHistogramTextSize(rootNode) {
  var hist = [];
  for(var i=0; i<256; ++i)
    hist.push(0);

  forEeachElement(rootNode, function(node) {
    var font_size = parseInt(window.getComputedStyle(node).fontSize);
    var chars = getNodeText(node).length;

    //saturate to <0, 255>
    font_size = Math.max(0, Math.min(font_size, hist.length - 1));

    hist[font_size] += chars;
  });

  return hist;
}


//todo move

function countWords(str) {
  var matches = str.match(/[\w\d]+/gi);
  return matches ? matches.length : 0;
}

function GetWordCountPerChildArithmeticMean(rootNode) {
  debugger;
  var word_sum = 0;
  var child_count = 0;

  for (var i = 0; i < rootNode.childElementCount; ++i) {
    var child = rootNode.children[i];
    if(!ignoreNodeTraversal(child)) {
      forEeachElement(child, function(node) {
          var text = getNodeText(node);
          word_sum += countWords(text);
      });
      child_count++;
    }
  }
  if (child_count < 1)
    return 0;

  return Math.round(word_sum / child_count);
}

function GetWordCountPerChildGeometricMean(rootNode) {
  debugger;
  var word_product = 1;
  var child_count = 0;
  var nonzero_child_count = 0;

  for (var i = 0; i < rootNode.childElementCount; ++i) {
    var child = rootNode.children[i];
    if(!ignoreNodeTraversal(child)) {
      var word_sum = 0;
      forEeachElement(child, function(node) {
          var text = getNodeText(node);
          word_sum += countWords(text);
      });
      if (word_sum != 0) {
        word_product *= word_sum;
        nonzero_child_count++;
      }
      child_count++;
    }
  }
  if (child_count < 1 || nonzero_child_count < 1)
    return 0;

  return Math.round(Math.pow(word_product,1/child_count));
}


function GetWordCountPerChild(rootNode) {
  return GetWordCountPerChildArithmeticMean(rootNode);
}