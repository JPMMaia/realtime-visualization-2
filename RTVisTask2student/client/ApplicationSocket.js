function ApplicationSocket(application, uri) {
	
	this.webSocket = new WebSocket(uri);
	var webSocketFunctions = {
		open: function (evt) {
			application.onSocketConnected();
		},
		close: function (evt) {
			setTimeout(function () { window.location.href = 'client.html'; }, 3000);
			document.write('Socket disconnected. Restarting..');
		},
		message: function (evt) {
			
			var dataLength = evt.data.byteLength;
			var dataview = new DataView(evt.data);
			var resolution = dataview.getInt32(dataLength - 8, true);
			var maxBin = dataview.getInt32(dataLength - 4, true);
			console.log("dataLength " + dataLength);
			console.log("max " + dataview.getInt32(dataLength - 4, true));
			console.log("res " + dataview.getInt32(dataLength - 8, true));
			
			var texureData = new Float32Array(evt.data.slice(0, dataLength - 8));
			application.render(texureData, maxBin);
			
		},
		error: function (evt) {
			console.log('error', evt);
		}
	};

	this.webSocket.binaryType = "arraybuffer";
	this.webSocket.onopen = webSocketFunctions.open;
	this.webSocket.onclose = webSocketFunctions.close;
	this.webSocket.onmessage = webSocketFunctions.message;
	this.webSocket.onerror = webSocketFunctions.error;
}

ApplicationSocket.prototype.setDataLimits = function (minX, maxX, minY, maxY) {
	this.webSocket.send("KDE " + Math.round(minX) + " " + Math.round(maxX) + " " + Math.round(minY) + " " + Math.round(maxY));
}
ApplicationSocket.prototype.requestImage = function () {
	this.webSocket.send("req_image");
}
