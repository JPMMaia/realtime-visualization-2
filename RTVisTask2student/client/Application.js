function Application(websocket)
{
	this.websocket = websocket;
	
	this.centerX = 200.0;
	this.centerY = 200.0;
	this.windowWidth = 600.0;
	this.windowHeight = 600.0;
	this.dirty = true;
}

Application.prototype.render = function()
{
	if(this.dirty)
	{	
		this.dirty = false;
		
		var halfWidth = this.windowWidth / 2.0;
		var halfHeight = this.windowHeight / 2.0;
		 
		var minX = this.centerX - halfWidth;
		var maxX = this.centerX + halfWidth;
		var minY = this.centerY + halfHeight;
		var maxY = this.centerY + halfHeight;
		
		// Send request to server in order to change the window of results:
		this.websock.send("KDE " + minX + " " + maxX + " " + minY + " " + maxY);
	}
	
	this.websocket.send("req_image");
};

Application.prototype.zoom = function(value)
{	
	this.windowWidth *= value;
	this.windowHeight *= value;
	this.dirty = true;
};
Application.prototype.pan = function(x, y)
{	
	this.centerX += x;
	this.centerY += y;
	this.dirty = true;
};
