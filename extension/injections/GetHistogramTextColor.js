function colorToInt(color) {
  var r = 0, g = 0, b = 0;
  var res;
  if ((res = /^rgba\(([\d]+),\s?([\d]+),\s?([\d]+),\s?([\d]+|[\d]*.[\d]+)\)/.exec(color)) ||
      (res = /^rgb\(([\d]+),\s?([\d]+),\s?([\d]+)\)/.exec(color))) {
    r = Number(res[1]);
    g = Number(res[2]);
    b = Number(res[3]);
    // Ignore possible alpha.
    // a = cache[4];
  }
  return 65536 * r + 256 * g + b;
}

function colorToGrayInt(color) {
  var r = 0, g = 0, b = 0;
  var res;
  if ((res = /^rgba\(([\d]+),\s?([\d]+),\s?([\d]+),\s?([\d]+|[\d]*.[\d]+)\)/.exec(color)) ||
      (res = /^rgb\(([\d]+),\s?([\d]+),\s?([\d]+)\)/.exec(color))) {
    r = Number(res[1]);
    g = Number(res[2]);
    b = Number(res[3]);
    // Ignore possible alpha.
    // a = cache[4];
  }
  return Math.round((r+g+b)/3);
}

function countTextPixels(node) {
  var style = window.getComputedStyle(node);
  var font = style.font;
  var fontSize = parseInt(style.fontSize);
  var text = getNodeText(node);

  // No text found or empty
  if (!text.trim())
    return 0;

  var canvas = document.createElement('canvas');
  var context = canvas.getContext("2d");
  context.font = font;
  canvas.width = context.measureText(text).width;
  canvas.height = fontSize * 2;
  context.font = font; // Somehow it returned to default.
  context.fillText(text, 0, fontSize);

  //Debug:
  //helperWindowAppend(canvas);

  // Something went wrong.
  if (canvas.width == 0 || canvas.height == 0) {
    console.log("warning: canvas 0 dimmension");
    return 0;
  }

  var pixelCount = 0;
  var imageData = context.getImageData(0,0,canvas.width,canvas.height);
  var data = imageData.data;
  for (var i=0; i < data.length; i+=4) {
    //var r = data[i];
    //var g = data[i + 1];
    //var b = data[i + 2];
    var a = data[i + 3];
    if(a > 0) // Font smoothing count threshold = 0.
      ++pixelCount;
  }

  return pixelCount;
}

window.HistogramTextColorsMap = {};

function GetHistogramTextColor(rootNode, colorsCount) {
  HistogramTextColorsMap = {};
  forEeachElement(rootNode, function(node) {
    var color = colorToInt(window.getComputedStyle(node).color);
    var pixels = countTextPixels(node);
    if (!HistogramTextColorsMap[color])
      HistogramTextColorsMap[color] = pixels;
    else
      HistogramTextColorsMap[color] += pixels;
  });

  var results = []; // pairs of [color, count]
  var pixelsCount = 0;
  for (color in HistogramTextColorsMap) {
    var count = HistogramTextColorsMap[color];
    pixelsCount += count;
    // For in loop does implicit key conversion (int -> string).
    results.push([Number(color), count]);
  }
  HistogramTextColorsMap = {}; // Free memory.

  results.sort(function(a, b) {
     return (a[1] < b[1]) ? 1 : (a[1] > b[1]) ? -1 : 0;
  });

  // For tests only.
  if (colorsCount == -1)
    colorsCount = results.length;

  // Trim.
  results.length = colorsCount;

  var colors = [];
  var shares = [];

  for (var i=0; i < results.length; ++i) {
    if (results[i]) {
      colors[i] = results[i][0];
      shares[i] = results[i][1] / pixelsCount;
    } else {
      colors[i] = -1;
      shares[i] =  0;
    }
  }
  return {"values" : colors, "shares" : shares};
}


function GetHistogramTextColorGrayscale(rootNode) {
  var hist = [];
  for(var i=0; i<256; ++i)
    hist.push(0);

  forEeachElement(rootNode, function(node) {
    var gray_color = colorToGrayInt(window.getComputedStyle(node).color);
    var pixels = countTextPixels(node);
    hist[gray_color] += pixels;

    if(gray_color < 0 || gray_color > 255)
      console.log("Gray color out of bounds!");
  });

  return hist;
}