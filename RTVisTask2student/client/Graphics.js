function Graphics(application) 
{
	this.margin = { top: 20, right: 20, bottom: 30, left: 40 };
    this.width = 500 - margin.left - margin.right;
    this.height = 500 - margin.top - margin.bottom;
	
	this.initializeAxis(application);
	this.initializeRenderer(application);
}

Graphics.prototype.render = function ()
{
	this.renderer.render(this.scene, this.camera);
};

Graphics.prototype.initializeAxis = function (application)
{	
	var margin = application.getWindowMargin();
	var width = application.getWindowWidth();
	var height = application.getWindowHeight(); 

	// SVG container for axes:
	this.axisSVG = d3.select("body").append("svg")
		.attr("width", width + margin.left + margin.right)
		.attr("height", height + margin.top + margin.bottom)
		.append("g")
		.attr("transform", "translate(" + margin.left + "," + margin.top + ")");
		
	this.updateAxis(application);
};
Graphics.prototype.updateAxis = function(application)
{
	var width = application.getWindowWidth();
	var height = application.getWindowHeight(); 
	var minX = application.getMinX();
	var maxX = application.getMaxX();
	var minY = application.getMinY();
	var maxY = application.getMaxY();
	
	var x = d3.scale.linear()
		.domain([minX, maxX])
		.range([0, width]);

	var y = d3.scale.linear()
		.domain([minY, maxY])
		.range([height, 0]);

	// d3 axes
	var xAxis = d3.svg.axis()
		.scale(x)
		.orient("bottom");

	var yAxis = d3.svg.axis()
		.scale(y)
		.orient("left");

	this.axisSVG.selectAll("*").remove();
	this.axisSVG.append("g")
		.attr("class", "x axis")
		.attr("transform", "translate(0," + height + ")")
		.attr("fill", "#888")
		.call(xAxis)
		.append("text")
		.attr("class", "label")
		.attr("x", width)
		.attr("y", -6)
		.style("text-anchor", "end")
		.text("departure delay (mins)");

	this.axisSVG.append("g")
		.attr("class", "y axis")
		.attr("fill", "#888")
		.call(yAxis)
		.append("text")
		.attr("class", "label")
		.attr("transform", "rotate(-90)")
		.attr("y", 6)
		.attr("dy", ".71em")
		.style("text-anchor", "end")
		.text("arrival delay (mins)");
};

Graphics.prototype.initializeRenderer = function(application)
{
	var margin = application.getWindowMargin();
	var width = application.getWindowWidth();
	var height = application.getWindowHeight(); 
	
	// Orthographic Camera:
	this.camera = new THREE.OrthographicCamera(-width / 2, width / 2, height / 2, -height / 2, 1, 1000);
	this.camera.position.z = 1000;

	// Scene:
	this.scene = new THREE.Scene();

	// Renderer:
	this.renderer = new THREE.WebGLRenderer();
	this.renderer.setSize(width, height);

	// Renderer placement:
	this.renderer.domElement.style.position = "absolute";
	this.renderer.domElement.style.top = (margin.top) + "px";
	this.renderer.domElement.style.left = (margin.left) + "px";
	this.renderer.domElement.style.zIndex = "1";

	// Append renderer to DOM:
	document.body.appendChild(this.renderer.domElement);
};
Graphics.prototype.initializeScene = function (textureData)
{	
	// Histogram data texture:
	var textureSize = textureData.length;
    var textureWidth = Math.sqrt(textureSize);
	var dataTexture = new THREE.DataTexture(textureData, textureWidth, textureWidth, THREE.LuminanceFormat, THREE.FloatType);
	dataTexture.wrapS = THREE.ClampToEdgeWrapping;
	dataTexture.wrapT = THREE.ClampToEdgeWrapping;
	dataTexture.repeat.set(1, 1);
	dataTexture.needsUpdate = true;
	
	// Quad geometry:
	var geometry = new THREE.PlaneGeometry(this.width, this.height, 1, 1);

	// Basic material with histogram data texture: 
	var material = new THREE.MeshBasicMaterial( {wireframe: false, map: dataTexture } );

	// Mesh:
	var mesh = new THREE.Mesh(geometry, material);
	
	// Scene:
	while(this.scene.children.length > 0)
	{
		this.scene.remove(this.scene.children[0]);
	}
	
	this.scene.add(mesh);
};
