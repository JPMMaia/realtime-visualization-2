function Application(onSocketConnected)
{
	this.applicationSocket = new ApplicationSocket(this, "ws://localhost:8001");
	this.onSocketConnected = function()
	{
		onSocketConnected();	
	};
	
	this.windowMargin = { top: 20, right: 20, bottom: 30, left: 40 };
	this.windowWidth = 500 - margin.left - margin.right;
	this.windowHeight = 500 - margin.top - margin.bottom;
	
	this.centerX = 200.0;
	this.centerY = 200.0;
	this.dataRangeWidth = 600.0;
	this.dataRangeHeight = 600.0;
	this.dataParametersDirty = true;
	this.graphics = new Graphics(this);
	
	this.dataDirty = true; 
}

Application.prototype.update = function()
{
	if(this.dataParametersDirty)
	{	
		this.dataParametersDirty = false;
		 
		var minX = this.getMinX();
		var maxX = this.getMaxX();
		var minY = this.getMinY();
		var maxY = this.getMaxY();
		
		this.applicationSocket.setDataLimits(minX, maxX, minY, maxY);
		this.applicationSocket.requestImage();
	}
};
Application.prototype.render = function(textureData)
{
	this.graphics.initializeScene(textureData);
	this.graphics.render();
};

Application.prototype.zoom = function(value)
{	
	this.dataRangeWidth *= value;
	this.dataRangeHeight *= value;
	this.dataParametersDirty = true;
};
Application.prototype.pan = function(x, y)
{	
	this.centerX -= x;
	this.centerY -= y;
	this.dataParametersDirty = true;
};

Application.prototype.getRangeDataWidth = function()
{
	return this.dataRangeWidth;
}
Application.prototype.getRangeDataHeight = function()
{
	return this.dataRangeHeight;
}
Application.prototype.getMinX = function()
{
	return this.centerX - (this.dataRangeWidth / 2.0);
};
Application.prototype.getMaxX = function()
{
	return this.centerX + (this.dataRangeWidth / 2.0);
};
Application.prototype.getMinY = function()
{
	return this.centerY - (this.dataRangeHeight / 2.0);
};
Application.prototype.getMaxY = function()
{
	return this.centerY + (this.dataRangeHeight / 2.0);
};

Application.prototype.getWindowMargin = function()
{
	return this.windowMargin;
};
Application.prototype.getWindowWidth = function()
{
	return this.windowWidth;
};
Application.prototype.getWindowHeight = function()
{
	return this.windowHeight;
};