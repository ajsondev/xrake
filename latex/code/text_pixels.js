function countTextPixels(node) {
  var style = window.getComputedStyle(node);
  var font = style.font;
  var fontSize = parseInt(style.fontSize);
  var text = getNodeText(node);

  if (!text.trim())
    return 0;

  var canvas = document.createElement('canvas');
  var context = canvas.getContext("2d");
  context.font = font;
  canvas.width = context.measureText(text).width;
  canvas.height = fontSize * 2;
  context.font = font;
  context.fillText(text, 0, fontSize);

  var pixelCount = 0;
  var imageData = context.getImageData(0, 0, 
    canvas.width, canvas.height);
  var data = imageData.data;
  for (var i=0; i < data.length; i += 4) {
    //var r = data[i];
    //var g = data[i + 1];
    //var b = data[i + 2];
    var alpha = data[i + 3];
    if(alpha > 0)
      ++pixelCount;
  }

  return pixelCount;
}