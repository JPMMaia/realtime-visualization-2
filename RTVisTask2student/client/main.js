var g_application;

var g_zoomScalar = 0.1;
var g_panScalar = 1.0;

function onZoomInButtonClick()
{	
	g_application.zoom(1.0 - g_zoomScalar);
}
function onZoomOutButtonClick()
{	
	g_application.zoom(1.0 + g_zoomScalar);
}

function onPanLeftButtonClick()
{	
	g_application.pan(-g_panScalar, 0.0);
}
function onPanRightButtonClick()
{	
	g_application.pan(g_panScalar, 0.0);
}
function onPanUpButtonClick()
{	
	g_application.pan(0.0, g_panScalar);
}
function onPanDownButtonClick()
{	
	g_application.pan(0.0, -g_panScalar);
}

function setupUI()
{
	var zoomInButton = document.getElementById("zoom_in");
	zoomInButton.onclick = onZoomInButtonClick;
	
	var zoomOutButton = document.getElementById("zoom_out");
	zoomOutButton.onclick = onZoomOutButtonClick;
	
	var panLeftButton = document.getElementById("pan_left");
	panLeftButton.onclick = onPanLeftButtonClick;
	
	var panRightButton = document.getElementById("pan_right");
	panRightButton.onclick = onPanRightButtonClick;
	
	var panUpButton = document.getElementById("pan_up");
	panUpButton.onclick = onPanUpButtonClick;
	
	var panDownButton = document.getElementById("pan_down");
	panDownButton.onclick = onPanDownButtonClick;
}

function setupInput()
{
	window.onkeydown = function(keyboardEvent)
    {
        g_application.getKeyboard().onKeyDown(keyboardEvent.keyCode);
    };
    window.onkeyup = function(keyboardEvent)
    {
        g_application.getKeyboard().onKeyUp(keyboardEvent.keyCode);
    };

	var canvas = g_application.getRendererCanvas();
    canvas.onmousedown = function(mouseEvent)
    {
        g_application.getMouse().onButtonDown(mouseEvent.button);
    };
    canvas.onmouseup = function(mouseEvent)
    {
        g_application.getMouse().onButtonUp(mouseEvent.button);
    };
    canvas.onmousemove = function(mouseEvent)
    {
		var mouse = g_application.getMouse();
		if(mouse.isButtonDown(0))
			g_application.pan(mouseEvent.movementX * 0.04, mouseEvent.movementY * 0.04);
    };
    canvas.onwheel = function(wheelEvent)
    {
		var zoomValue = wheelEvent.wheelDelta > 0 ? 0.9 : 1.1;
        g_application.zoom(zoomValue);
    };
}

function run()
{
	g_application.update();

    setTimeout(
        function()
        {
            window.requestAnimationFrame(run);
        },
        200
    );
}

function initializeApplication() 
{
	g_application = new Application(run);
	
	setupUI();
	setupInput();
}

window.addEventListener("load", initializeApplication, false);