function GetRect(node) {
  var minX = Infinity;
  var minY = Infinity;
  var maxX = 0;
  var maxY = 0;
  //forEeachElement(node, function(node) {
    var rc = node.getBoundingClientRect();
    minX = Math.min(minX, rc.left);
    minY = Math.min(minY, rc.top);
    maxX = Math.max(maxX, rc.right);
    maxY = Math.max(maxY, rc.bottom);
  //});

  var realWidth = (maxX - minX);
  var realHeight = (maxY - minY);

  var realX = minX;
  var realY = minY;

  var scrollLeft = 0;
  var scrollTop = 0;
  while(node && node != document) {
    scrollLeft += node.scrollLeft;
    scrollTop += node.scrollTop;
    node = node.parentNode;
  }

  realX += scrollLeft;
  realY += scrollTop;

  //return {"x" : realX, "y" : realY, "w" : realWidth, "h" : realHeight};
  return [realX, realY, realWidth, realHeight];
}

function GetNormalizedRect(node) {
  var r = GetRect(node);
  // Transform to fraction
  r[0] /= document.body.clientWidth;
  r[1] /= document.body.clientHeight;
  r[2] /= document.body.clientWidth;
  r[3] /= document.body.clientHeight;

  // Trim to <0,1>
  r[0] = range_0_1(r[0]);
  r[1] = range_0_1(r[1]);
  r[2] = range_0_1(r[2]);
  r[3] = range_0_1(r[3]);

  return r;
}


// TODO: move

var C_MAX_INT = 2147483647;

function variance(arr)
{
  var len = 0;
  var sum=0;
  for(var i=0;i<arr.length;i++) {
   len++;
   sum += parseFloat(arr[i]);
  }

  var v = 0;
  if (len <= 1)
     return C_MAX_INT;

  var mean = sum / len;
  for(var i=0;i<arr.length;i++) {
    v += (arr[i] - mean) * (arr[i] - mean);      
  }
  
  return v / len;
}

function GetChildrenRectVariance(rootNode) {
  var x = [];
  var y = [];
  var w = [];
  var h = [];
  for (var i = 0; i < rootNode.childElementCount; ++i) {
    var child = rootNode.children[i];
    if(!ignoreNodeTraversal(child)) {
      var r = GetRect(child);
      if (r[2] != 0 && r[3] != 0) {
        //element has width and height
        x.push(r[0]);
        y.push(r[1]);
        w.push(r[2]);
        h.push(r[3]);
      }
    }
  }

  return [
    variance(x),
    variance(y),
    variance(w),
    variance(h)
  ];
}