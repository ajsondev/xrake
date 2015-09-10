function HideAll() {
  var hidden = (document.body.style.visibility === "hidden");
  if (!hidden) {
    document.body.style.visibility = "hidden";
  } else {
    // switching only for tests
    document.body.style.visibility = "visible";
  }
  return {};
}
