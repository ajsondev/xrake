
function WarcDiscoveryGetPages() {
	var urls = [];
	var childs = document.querySelectorAll("li a");
	for (var i=0; i < childs.length; ++i)
	  urls.push(childs[i].href);
	return urls;
}

if (document.location.href === "http://localhost:8090/replay/") {
	// archive index
	console.log("WARC index");

	setInterval(function() {
		chrome.runtime.sendMessage({finalizeGetWarcLoadedPages: WarcDiscoveryGetPages()});
	}, 1000);

} else if(window.top == window.self) {

	// remove pywb banner
	function remove_banner() {
		var el = document.querySelector("wb_div");
		el && el.parentNode.removeChild(el);
	};
	document.addEventListener("readystatechange", remove_banner);
	
	//chrome.runtime.sendMessage({injectAllFunctionsToTrainingPage: {}});
}

console.log("WARC discovery /\\");