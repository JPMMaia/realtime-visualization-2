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
}

window.addEventListener("load", initializeApplication, false);