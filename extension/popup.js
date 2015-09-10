var display;

function setDisplay(str) {
	console.log(str);
	display.innerText = str;
}

// TODO: each action in new connection
function performAction(text) {
chrome.runtime.sendMessage({action: text});
}


document.addEventListener('DOMContentLoaded', function () {
	display = document.getElementById("last-response");

	var els = document.querySelectorAll(".action");
	for (var i=0; i<els.length; ++i)
		els[i].addEventListener('click', performAction.bind(null, els[i].getAttribute("actionName")));

	var close_els = document.querySelectorAll("[closeBehaviour]");
	for (var i=0; i<close_els.length; ++i)
		close_els[i].addEventListener('click', function(){window.close()});

	chrome.runtime.onMessage.addListener(
	  function(request, sender) {
		if ('text' in request) {
			setDisplay(request.text);
		}
	  });
});
