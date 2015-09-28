//private
var selectedNodes = [];
function reportResult() {
	//Build json
	var json = [];
	for(var i=0; i<selectedNodes.length; ++i) {
		var marker = markerFromNode(selectedNodes[i]);
		//marker["type"] = "list"; // for now only supported type
		json.push(marker);
	}
	console.log("JSON result: ", json);
	chrome.runtime.sendMessage({finalizeGetTrainingJson: json});
}

//private
var currentMarkingRect;
function SetMarkingRect(node) {
  if (!node)
  	return;
  var r = document.createElement("div");
  var rect  = GetRect(node);
  r.style.zIndex = 999999999;
  r.style.background = 'red';
  r.style.opacity = 0.2;
  r.style.position = "absolute";
  r.style.left = rect[0] + "px";
  r.style.top = rect[1] + "px";
  r.style.width = rect[2] + "px";
  r.style.height = rect[3] + "px";
  r.setAttribute("matches-Xrake-Uid", uidFromNode(node));
  r.style.pointerEvents = "none";

  if (currentMarkingRect)
  	document.body.removeChild(currentMarkingRect);
  currentMarkingRect = document.body.appendChild(r);
}

var currentSelection;
function GetTrainingJson() {
	selectedNodes = [];

	document.addEventListener("keypress", function(e) {
		var key = String.fromCharCode(e.keyCode);
		if (key == "a") {
			// Mark as 'list' type
			currentMarkingRect = null; // Prevent deleting current marking rect.
			selectedNodes.push(currentSelection);
		} else if(key == "f") {
			// Finish and send json to server
			reportResult();
		} else if(key == "x") {
			currentSelection = currentSelection && currentSelection.parentNode;
			SetMxarkingRect(currentSelection);
		} else if(key == "z") {
			var node = selectedNodes.pop();
			if (node) {
				var rect = document.querySelector("[matches-Xrake-Uid='"+uidFromNode(node)+"']");
				rect.parentNode.removeChild(rect);
			}
		}

		console.log("keypress ", key);
	});

	forEeachElement(document.body, function(node) {
		node.addEventListener("mousemove", function(e) {
			var target = e.target;
			//var target = e.fromElement;

			if (!target || target === currentSelection)
				return;

			//DEBUGGING
			// if (target.getAttribute("data-xrake-uid") == "245")
			// 	debugger;
			console.log("mouse uid=", target.getAttribute("data-xrake-uid"));

			currentSelection = target;
			SetMarkingRect(currentSelection);
		});
	});
	
	console.log("GetTrainingJson loaded!");
	console.log("A -> Mark current selection as LIST");
	console.log("F -> Finish");
	console.log("X -> Select parent node");
	console.log("Z -> Remove last selection");
}

//Run!
GetTrainingJson();